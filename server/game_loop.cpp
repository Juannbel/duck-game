#include "game_loop.h"

#include <chrono>
#include <cmath>
#include <cstdint>
#include <random>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>
#include "common/blocking_queue.h"
#include "common/commands.h"
#include "common/config.h"
#include "common/map.h"
#include "common/snapshot.h"

using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

static Config& config = Config::get_instance();

const static int TICKS = config.get_server_ticks();
const milliseconds RATE(1000 / TICKS);
const uint its_after_round = 3000 / (1000 / TICKS);
const uint8_t ROUNDS_TO_WIN = config.get_rounds_to_win();
const uint8_t ROUNDS_BETWEEN_STATS = config.get_rounds_between_stats();
const std::string FIRST_MAP = "/server/lobby.yaml";

GameLoop::GameLoop(Queue<struct action>& game_queue, QueueListMonitor& queue_list):
        actions_queue(game_queue),
        snaps_queue_list(queue_list),
        round_number(ROUNDS_BETWEEN_STATS),
        round_finished(),
        game_finished(),
        first_round(true),
        paths_to_maps(map_loader.list_maps(MAPS_PATH)),
        curr_map(),
        game_initialized(false) {
    for (uint8_t i = MAX_DUCKS; i > 0; i--) {
        ducks_id_available.push_back(i - 1);
    }
}

std::string get_rand_string(const std::vector<std::string>& v_strings) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, v_strings.size() - 1);  // 0 es None
    return v_strings[static_cast<int>(dis(gen))];
}

void GameLoop::initialice_new_round() {
    if (first_round) {
        std::string lobby = DATA_PATH;
        lobby.append(FIRST_MAP);
        curr_map = map_loader.load_map(lobby);
        
    } else {
        curr_map = map_loader.load_map(get_rand_string(paths_to_maps));
    }
    std::lock_guard<std::mutex> lock(map_lock);
    game_operator.initialize_game(curr_map, ducks_info, first_round);
}

void GameLoop::run() {
    game_initialized = true;
    initialice_new_round();

    uint it = its_after_round;
    auto t1 = high_resolution_clock::now();
    initial_snapshot();
    while (_keep_running && (!game_finished || it)) {
        pop_and_process_all();
        send_game_status(t1, it);
        auto t2 = high_resolution_clock::now();
        milliseconds duration = duration_cast<milliseconds>(t2 - t1);
        milliseconds rest = RATE - duration;
        if (RATE < duration) {
            milliseconds behind = duration - rest;
            milliseconds lost = behind - behind % RATE;
            t1 += lost;

        } else {
            std::this_thread::sleep_for(rest);
        }
        t1 += RATE;
    }

    round_finished = true;
    game_finished = true;
    it = 0;
    round_number = 0;
    create_and_push_snapshot(it);

    _is_alive = false;
}

void GameLoop::initial_snapshot() {
    Snapshot actual_status = {};
    game_operator.get_snapshot(actual_status);
    actual_status.maps.push_back(curr_map.map_dto);
    actual_status.round_finished = false;
    add_rounds_won(actual_status);
    push_responce(actual_status);
}

void GameLoop::pop_and_process_all() {
    action action{};
    while (actions_queue.try_pop(action)) {
        std::lock_guard<std::mutex> lock(map_lock);
        game_operator.process_action(action);
    }
    game_operator.update_game_status();
}

void GameLoop::create_and_push_snapshot(const uint& its_since_finish) {
    Snapshot actual_status = {};
    game_operator.get_snapshot(actual_status);
    if (first_round)
        round_finished = game_operator.check_start_game();
    check_for_winner(actual_status);

    actual_status.round_finished = its_since_finish == 0 ? round_finished : false;
    actual_status.show_stats = its_since_finish == 0 ? round_number == 0 : false;
    actual_status.game_finished = its_since_finish == 0 ? game_finished : false;
    add_rounds_won(actual_status);
    push_responce(actual_status);
}

void GameLoop::send_game_status(auto& t1, uint& its_since_finish) {
    create_and_push_snapshot(its_since_finish);
    if (!its_since_finish) {
        first_round = false;
        if (!game_finished)
            wait_ready();
        t1 = high_resolution_clock::now();
        action action{};
        while (actions_queue.try_pop(action)) {}
        round_number = !round_number && !game_finished ? ROUNDS_BETWEEN_STATS : round_number;
        its_since_finish = its_after_round;
        round_finished = false;
    } else if (round_finished) {
        --its_since_finish;
    }
}

void GameLoop::add_rounds_won(Snapshot& snapshot) {
    for (auto& duck: snapshot.ducks) {
        duck.rounds_won = winners_id_count[duck.duck_id];
    }
}

void GameLoop::push_responce(const Snapshot& actual_status) {
    snaps_queue_list.send_to_every(actual_status);
}

void GameLoop::check_for_winner(const Snapshot& actual_status) {
    if (round_finished || first_round) {
        return;
    }
    uint8_t winner_id;
    uint8_t players_alive = 0;
    for (auto const& duck: actual_status.ducks) {
        if (!duck.is_dead) {
            winner_id = duck.duck_id;
            ++players_alive;
        }
    }
    if (!players_alive) {
        round_finished = true;
        --round_number;
    }
    if (players_alive == 1 && !round_finished) {
        winners_id_count[winner_id]++;
        round_finished = true;
        --round_number;
    }
    if (round_number == 0) {
        uint8_t max_winner = ROUNDS_TO_WIN;
        for (auto& [id, count]: winners_id_count) {
            if (count < max_winner) {
                continue;
            }
            if (game_finished && count == max_winner) {
                game_finished = false;
            } else {
                game_finished = true;
                max_winner = count;
            }
        }
    }
}

uint8_t GameLoop::add_player(const std::string& player_name) {
    std::lock_guard<std::mutex> lock(map_lock);
    if (ducks_info.size() >= MAX_DUCKS) {
        throw std::runtime_error("Exceso de jugadores");
    }
    uint8_t duck_id = ducks_id_available.back();
    ducks_id_available.pop_back();
    ducks_info.emplace_back(duck_id, player_name);
    initial_snapshot();
    return duck_id;
}

void GameLoop::delete_duck(const uint8_t duck_id) {
    std::lock_guard<std::mutex> lock(map_lock);
    game_operator.delete_duck_player(duck_id);
    auto it = std::find_if(ducks_info.begin(), ducks_info.end(),
                           [duck_id](const auto& info) { return info.first == duck_id; });
    if (it != ducks_info.end()) {
        ducks_info.erase(it);
        ducks_id_available.push_back(duck_id);
    }

    if (ducks_info.size() <= 1 && game_initialized) {
        _keep_running = false;
    }
}

void GameLoop::start_game() {
    game_operator.initialize_boxes(curr_map);
}

void GameLoop::stop() {
    _keep_running = false;
}

void GameLoop::notify_not_started() {
    Snapshot status = {};
    status.game_finished = true;
    status.round_finished = true;
    status.show_stats = true;
    status.maps.push_back(curr_map.map_dto);

    // es necesario enviar 1 para desbloquearlo de la waiting screen
    push_responce(status);
}

GameLoop::~GameLoop() {
    actions_queue.close();
}

void GameLoop::sleep_checking(const milliseconds& time) {
    uint its_to_sleep = time / RATE;
    while (its_to_sleep && _keep_running) {
        --its_to_sleep;
        std::this_thread::sleep_for(RATE);
    }
}

void GameLoop::create_new_map(std::map<uint8_t, uint8_t> &players_readys) {
    std::lock_guard<std::mutex> lock(map_lock);
    for (auto [id, name]: ducks_info) {
        players_readys[id] = 0;
    }
}

void GameLoop::wait_ready() {
    uint8_t readys = 0;
    std::map<uint8_t, uint8_t> players_readys;
    create_new_map(players_readys);
    bool first_ready = true;
    while (readys < ducks_info.size() && _keep_running) {
        action action{};
        while (actions_queue.try_pop(action)) {
            if (action.command != Ready)
                continue;
            if (players_readys[action.duck_id] == 0 && first_ready) {
                players_readys[action.duck_id] = 1;
                ++readys;
            }
            if (players_readys[action.duck_id] == 1 && !first_ready) {
                players_readys[action.duck_id] = 2;
                ++readys;
            }
        }
        if (readys == ducks_info.size() && first_ready) {
            if (!round_number)
                sleep_checking(milliseconds(3000));
            readys = 0;
            first_ready = false;
            initialice_new_round();
            initial_snapshot();
        }
        std::this_thread::sleep_for(RATE);
    }
}

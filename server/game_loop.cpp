#include "game_loop.h"

#include <chrono>
#include <cmath>
#include <cstdint>
#include <random>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include "common/snapshot.h"
#include "game/ticks.h"

using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

const milliseconds RATE(1000 / TICKS);
const uint its_after_round = 3000 / (1000 / TICKS);

GameLoop::GameLoop(Queue<struct action>& game_queue, QueueListMonitor& queue_list):
        actions_queue(game_queue),
        snaps_queue_list(queue_list),
        round_number(5),
        round_finished(),
        game_finished(),
        paths_to_maps(map_loader.list_maps(SERVER_DATA_PATH)),
        curr_map() {}

std::string get_rand_string(const std::vector<std::string>& v_strings) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, v_strings.size() - 1);  // 0 es None
    return v_strings[static_cast<int>(dis(gen))];
}

void GameLoop::initialice_new_round() {
    curr_map = map_loader.load_map(get_rand_string(paths_to_maps));
    game_operator.initialize_game(curr_map, ducks_info);
}

void GameLoop::run() {

    initialice_new_round();
    initial_snapshot();

    // Tiempo para que los jugadores vean que pato les toco
    std::this_thread::sleep_for(milliseconds(4000));

    uint it = its_after_round;
    auto t1 = high_resolution_clock::now();
    initial_snapshot();
    while (_keep_running && (!game_finished || it)) {
        pop_and_process_all();
        create_and_push_snapshot(t1, it);
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
    if (game_finished) {
        create_and_push_snapshot(t1, it);
    }

    _is_alive = false;

    // Caso en el que se cierra el gameloop, si hay algun cliente se le avisa que termino el juego
    Snapshot actual_status = {};
    game_operator.get_snapshot(actual_status);
    actual_status.game_finished = true;
    push_responce(actual_status);
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
        game_operator.process_action(action);
    }
    game_operator.update_game_status();
}

void GameLoop::create_and_push_snapshot(auto& t1, uint& its_since_finish) {
    Snapshot actual_status = {};
    game_operator.get_snapshot(actual_status);
    check_for_winner(actual_status);

    actual_status.round_finished = its_since_finish == 0 ? round_finished : false;
    actual_status.show_stats = its_since_finish == 0 ? round_number == 0 : false;
    actual_status.game_finished = its_since_finish == 0 ? game_finished : false;
    add_rounds_won(actual_status);
    push_responce(actual_status);
    if (!its_since_finish) {
        std::this_thread::sleep_for(milliseconds(3000));
        initialice_new_round();
        initial_snapshot();
        t1 = high_resolution_clock::now();
        action action{};
        while (actions_queue.try_pop(action)) {}
        round_number = !round_number && !game_finished ? 5 : round_number;
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
    if (round_finished) {
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
        for (auto& [id, count]: winners_id_count) {
            uint8_t max_winner = 10;
            if (count >= max_winner) {
                if (game_finished && count == max_winner) {
                    game_finished = false;
                } else {
                    game_finished = true;
                    max_winner = count;
                }
            }
        }
    }
}

uint8_t GameLoop::add_player(const std::string& player_name) {
    if (ducks_info.size() >= MAX_DUCKS) {
        throw std::runtime_error("Exceso de jugadores");
    }
    uint8_t duck_id = ducks_info.size();
    ducks_info.emplace_back(duck_id, player_name);
    return duck_id;
}

void GameLoop::delete_duck(const uint8_t duck_id) {
    game_operator.delete_duck_player(duck_id);
    auto it = std::find_if(ducks_info.begin(), ducks_info.end(),
                           [duck_id](const auto& info) { return info.first == duck_id; });
    if (it != ducks_info.end()) {
        ducks_info.erase(it);
    }

    if (ducks_info.size() <= 1) {
        _keep_running = false;
        if (on_game_end_callback) {
            std::cout << "Game ended" << std::endl;
            on_game_end_callback();
        }
    }
}

void GameLoop::set_on_game_end_callback(std::function<void()> callback) {
    on_game_end_callback = std::move(callback);
}

GameLoop::~GameLoop() {
    _keep_running = false;
    actions_queue.close();
}

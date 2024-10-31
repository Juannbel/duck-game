#include "game_loop.h"

#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <stdexcept>

#include "common/snapshot.h"
#include "game/ticks.h"

using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

const milliseconds RATE(1000 / TICKS);

GameLoop::GameLoop(Queue<struct action>& game_queue, QueueListMonitor& queue_list):
        actions_queue(game_queue),
        snaps_queue_list(queue_list),
        game_operator(),
        match_number(10),
        players_quantity() {}

void GameLoop::initialice_new_round(std::string path) {
    curr_map_dto = map_loader.loadMap(path);
    game_operator.initialize_game(curr_map_dto, players_quantity);
}

void GameLoop::run() {
    initialice_new_round(SERVER_DATA_PATH "/map1.yaml");
    auto t1 = high_resolution_clock::now();
    // uint it = 0;
    initial_snapshot();
    while (_keep_running) {
        pop_and_process_all();
        create_and_push_snapshot(t1);
        auto t2 = high_resolution_clock::now();
        milliseconds duration = duration_cast<milliseconds>(t2 - t1);
        milliseconds rest = RATE - duration;
        if (RATE < duration) {
            milliseconds behind = duration - rest;
            milliseconds lost = behind - behind % RATE;
            t1 += lost;
            // it += floor(lost / RATE);
        } else {
            std::this_thread::sleep_for(rest);
        }
        t1 += RATE;
        // ++it;
    }
}

void GameLoop::initial_snapshot() {
    Snapshot actual_status = {};
    game_operator.get_snapshot(actual_status);
    actual_status.maps.push_back(curr_map_dto);
    actual_status.match_finished = false;
    push_responce(actual_status);
}

void GameLoop::pop_and_process_all() {
    struct action action;
    while (actions_queue.try_pop(action)) {
        game_operator.process_action(action);
    }
    game_operator.update_game_status();
}

void GameLoop::create_and_push_snapshot(auto& t1) {
    Snapshot actual_status = {};
    game_operator.get_snapshot(actual_status);
    bool round_finished = check_for_winner(actual_status);
    actual_status.match_finished = round_finished;
    push_responce(actual_status);
    if (round_finished) {
        std::this_thread::sleep_for(milliseconds(3000));
        initialice_new_round(SERVER_DATA_PATH "/map2.yaml");
        initial_snapshot();
        t1 = high_resolution_clock::now();
        struct action action;
        while (actions_queue.try_pop(action)) {}
    }
}

void GameLoop::push_responce(Snapshot& actual_status) {
    snaps_queue_list.send_to_every(actual_status);
}


bool GameLoop::check_for_winner(Snapshot& actual_status) {
    uint8_t winner_id;
    uint8_t players_alive = 0;
    for (auto& duck: actual_status.ducks) {
        if (!duck.is_dead) {
            winner_id = duck.duck_id;
            ++players_alive;
        }
    }
    if (players_alive == 1) {
        uint8_t i = 0;
        for (auto& [id, wins]: winners_id_count) {  // VALIDACION PARA QUE NO SE SUME MAS DE UNA VEZ
            i += wins;                              // PROVOSORIO, SOLO COMO TEST
        }
        if (i == match_number) {
            return false;
        }

        if (winners_id_count.find(winner_id) == winners_id_count.end()) {
            winners_id_count[winner_id] = 0;
        }
        ++winners_id_count[winner_id];
        std::cout << "Gano el jugador: " << +winner_id << " con un total de "
                  << +winners_id_count[winner_id] << " partidas ganadas " << std::endl;
        
        return true;
    }
    return false;
}

uint8_t GameLoop::add_player() {
    if (players_quantity >= MAX_DUCKS) {
        throw std::runtime_error("Exceso de jugadores");
    }
    return players_quantity++;
}

GameLoop::~GameLoop() {}

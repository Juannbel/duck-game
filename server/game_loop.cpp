#include "game_loop.h"

#include <chrono>
#include <cmath>
#include <cstdint>
#include <stdexcept>

#include "common/snapshot.h"
#include "game/ticks.h"

using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

const milliseconds RATE(1000 / TICKS);

GameLoop::GameLoop(Queue<struct action>& game_queue, QueueListMonitor& queue_list, Map& map_dto):
        actions_queue(game_queue),
        snaps_queue_list(queue_list),
        game_operator(map_dto),
        match_number(1),
        players_quantity() {}

void GameLoop::run() {
    game_operator.initialize_players(players_quantity);
    auto t1 = high_resolution_clock::now();
    // uint it = 0;
    while (_keep_running) {
        pop_and_process_all();
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

void GameLoop::pop_and_process_all() {
    struct action action;
    while (actions_queue.try_pop(action)) {
        game_operator.process_action(action);
    }
    game_operator.update_game_status();
    Snapshot actual_status = {};
    game_operator.get_snapshot(actual_status);
    check_for_winner(actual_status);
    push_responce(actual_status);
}

void GameLoop::push_responce(Snapshot& actual_status) {
    snaps_queue_list.send_to_every(actual_status);
}


void GameLoop::check_for_winner(Snapshot& actual_status) {
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
            return;
        }

        if (winners_id_count.find(winner_id) == winners_id_count.end()) {
            winners_id_count[winner_id] = 0;
        }
        ++winners_id_count[winner_id];
        std::cout << "Gano el jugador: " << +winner_id << " con un total de "
                  << +winners_id_count[winner_id] << " partidas ganadas " << std::endl;
        //
        // CARGAR SIGUIENTE MAPA EN LA SNAPSHOT
        // CARGAR EL NUEVO MAPA EN LAS ESTRUCTURAS
    }
}

uint8_t GameLoop::add_player() {
    if (players_quantity >= MAX_DUCKS) {
        throw std::runtime_error("Exceso de jugadores");
    }
    return players_quantity++;
}

GameLoop::~GameLoop() {}

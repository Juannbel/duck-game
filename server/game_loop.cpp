#include "game_loop.h"

#include <chrono>
#include <cmath>
#include <utility>
#include <vector>

#include "common/snapshot.h"
#include "server/duck_player.h"
#include "server/entitys_manager.h"

using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

#define TICKS 30
const milliseconds RATE(1000 / TICKS);
const uint8_t JUMP_IT = 60;
const float DUCK_SPEED = 4;
const float FALL_SPEED = 1;
const int32_t NEAR_CELLS = 3;

GameLoop::GameLoop(Queue<struct action>& game_queue, QueueListMonitor& queue_list,
                   Map &map_dto):
        actions_queue(game_queue), snaps_queue_list(queue_list), game_status(), ducks_info(), entity_manager(map_dto) {

    spawn_points = {
        {50, 50},
        {MAP_WIDTH_PIXELS - 50, 50},
        {50, 200},
        {MAP_WIDTH_PIXELS - 50, 200}
    };

}

void GameLoop::run() {
    auto t1 = high_resolution_clock::now();
    uint it = 0;
    while (_keep_running) {
        pop_and_process_all();
        auto t2 = high_resolution_clock::now();
        milliseconds duration = duration_cast<milliseconds>(t2 - t1);
        milliseconds rest = RATE - duration;
        if (RATE < duration) {
            milliseconds behind = duration - rest;
            milliseconds lost = behind - behind % RATE;
            t1 += lost;
            it += floor(lost / RATE);
        } else {
            std::this_thread::sleep_for(rest);
        }
        t1 += RATE;
        ++it;
    }
}

void GameLoop::pop_and_process_all() {
    struct action action;
    while (actions_queue.try_pop(action)) {
        process_action(action);
    }
    update_game_status();
    verify_spawn();
    push_responce();
}

void GameLoop::process_action(struct action& action) {
    auto it = player_to_duck_id.find(action.duck_id);
    if (it == player_to_duck_id.end()) {
        // Log error o maneja la acción inválida
        return;
    }
    int local_duck_id = it->second;
    DuckPlayer& player = ducks_info[local_duck_id];
    player.update_status(action.command);
}


void GameLoop::update_game_status() {
    int i = 0;
    for (auto& duck: ducks_info) {
        if (game_status.ducks[i].is_dead) {
            continue;
        }
        game_status.ducks[i] = duck.move_duck(entity_manager);
        // disparar
        ++i;
    }
    // Actualizar la posicion de las balas y vida de los patos si les pegan
}

void GameLoop::verify_spawn() {
    // Si paso el tiempo suficiente, genera algo en ese spawn
}

void GameLoop::push_responce() { snaps_queue_list.send_to_every(game_status); }

int GameLoop::add_player(int player_id) {
    int local_duck_id = player_to_duck_id.size();
    if (local_duck_id >= MAX_DUCKS) {
        throw std::runtime_error("GameLoop is full");
    }
    game_status.players_quantity++;
    player_to_duck_id[player_id] = local_duck_id;
    game_status.ducks[local_duck_id].duck_id = local_duck_id;

    auto& duck = ducks_info[local_duck_id];
    duck.set_coordenades_and_id(spawn_points[local_duck_id].first, spawn_points[local_duck_id].second, local_duck_id);
    return local_duck_id;
}


GameLoop::~GameLoop() {}

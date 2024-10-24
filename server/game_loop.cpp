#include "game_loop.h"

#include <chrono>
#include <cmath>

#include "common/snapshot.h"
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
                   Map &map_dto, uint8_t players_quantity):
        actions_queue(game_queue), snaps_queue_list(queue_list), game_status(), ducks_info(), entity_manager(map_dto) {

    game_status.players_quantity = players_quantity;
    uint8_t i = 0;
    for (auto& duck: ducks_info) {
        game_status.ducks[i].duck_id = i;
        if (i < players_quantity) {
            game_status.ducks[i] = duck.set_coordenades_and_id(50, 50 - DUCK_HITBOX_HEIGHT, i);
        }
        ++i;
    }
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
    DuckPlayer& player = ducks_info[action.duck_id];
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

GameLoop::~GameLoop() {}

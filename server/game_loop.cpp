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

GameLoop::GameLoop(Queue<struct action>& game_queue, QueueListMonitor& queue_list, Map& map_dto,
                   uint8_t players_quantity):
        players_quantity(players_quantity),
        actions_queue(game_queue),
        snaps_queue_list(queue_list),
        ducks_info(),
        entity_manager(map_dto) {
    uint8_t i = 0;
    for (auto& duck: ducks_info) {
        if (i < players_quantity) {
            duck.set_coordenades_and_id(50, 50 - DUCK_HITBOX_HEIGHT, i);
        }
        ++i;
    }
    //game_status.guns[0] = {1, Ak47, 150, 150};
    //entity_manager.add_gun(game_status.guns[0]);
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
    for (auto& duck: ducks_info) {
        duck.move_duck(entity_manager);
        duck.pickup(entity_manager);
    }
    // Actualizar la posicion de las balas y vida de los patos si les pegan
}

void GameLoop::verify_spawn() {
    // Si paso el tiempo suficiente, genera algo en ese spawn
}

void GameLoop::push_responce() { 
    Snapshot actual_status = {};
    actual_status.players_quantity = players_quantity;
    int i = 0;
    for (auto &duck : ducks_info) {
        actual_status.ducks[i] = duck.get_status();
        ++i;
    }
    entity_manager.add_guns_bullets_to_snapshot(actual_status);
    
    snaps_queue_list.send_to_every(actual_status); 
}

GameLoop::~GameLoop() {}

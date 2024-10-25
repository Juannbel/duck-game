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

GameLoop::GameLoop(Queue<struct action>& game_queue, QueueListMonitor& queue_list, Map& map_dto,
                   uint8_t players_quantity):
        actions_queue(game_queue),
        snaps_queue_list(queue_list),
        entity_manager(map_dto, players_quantity) {}

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
        entity_manager.process_action(action);
    }
    entity_manager.update_game_status();
    push_responce();
}

void GameLoop::push_responce() {
    Snapshot actual_status = {};
    entity_manager.get_snapshot(actual_status);
    snaps_queue_list.send_to_every(actual_status);
}

GameLoop::~GameLoop() {}

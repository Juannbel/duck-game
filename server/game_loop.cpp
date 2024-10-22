#include "game_loop.h"
#include <cmath>
#include <chrono>
using namespace std::chrono;

#define TICKS 30
const milliseconds RATE(1000/TICKS);
const uint8_t JUMP_IT = 60;
const float DUCK_SPEED = 4;
const float FALL_SPEED = 1;
const int32_t NEAR_CELLS = 3;

GameLoop::GameLoop(Queue<struct action>& game_queue, QueueListMonitor& queue_list, uint8_t players_quantity) : 
    actions_queue(game_queue), snaps_queue_list(queue_list) , game_status(), ducks_info(){
    game_status.players_quantity = players_quantity;
    uint8_t i = 0;
    for(auto &duck : ducks_info){
        game_status.ducks[i].duck_id = i;
        if (i < players_quantity) {
            game_status.ducks[i] = duck.set_coordenades_and_id(50, 50-DUCK_HITBOX_HEIGHT, i);
        }
        ++i;
    }
    load_map();
}

void GameLoop::load_map() {
    for (int16_t i = 0; i < MAP_HEIGHT_BLOCKS; ++i) {
        for (int16_t j = 0; j < MAP_WIDTH_BLOCKS; ++j) {
            map_blocks_info.blocks[i][j] = Empty;           
        }
        map_blocks_info.blocks[0][i] = Wall;
        map_blocks_info.blocks[MAP_WIDTH_BLOCKS-1][i] = Wall;
        entity_manager.add_block(0, i*BLOCK_SIZE);
        entity_manager.add_block(MAP_WIDTH_PIXELS-BLOCK_SIZE, i*BLOCK_SIZE);
    }
    int32_t floor_index = MAP_HEIGHT_BLOCKS-1;
    for (int32_t i = 0; i < MAP_WIDTH_BLOCKS; i++) { // Agrego pisos ficticias desde (0,160) a (560, 160)
        map_blocks_info.blocks[floor_index][i] = Floor;
        entity_manager.add_block(i*BLOCK_SIZE, floor_index*BLOCK_SIZE);
    }
    floor_index = 0;
    for (int32_t i = 0; i < MAP_WIDTH_BLOCKS; i++) {  //Agrego pisos ficticias desde (0,80) a (525, 80)
        map_blocks_info.blocks[floor_index][i] = Floor;
        entity_manager.add_block(i*BLOCK_SIZE, floor_index*BLOCK_SIZE);
    }
}

void GameLoop::run(){
    auto t1 = high_resolution_clock::now();
    uint it = 0;
    while(_keep_running){
        pop_and_process_all();
        auto t2 = high_resolution_clock::now();
        milliseconds duration = duration_cast<milliseconds>(t2 - t1);
        milliseconds rest = RATE - duration;
        if (RATE < duration){
            milliseconds behind = duration - rest; 
            milliseconds lost = behind - behind % RATE;
            t1 += lost;
            it += floor(lost / RATE);  
        }else {
            std::this_thread::sleep_for(rest);
        }
        t1 += RATE;
        ++it;
    }
}

void GameLoop::pop_and_process_all() {
    struct action action;
    while (actions_queue.try_pop(action)){
        process_action(action);
    }
    update_game_status();
    verify_spawn();
    push_responce();
}

void GameLoop::process_action(struct action& action) {
    DuckPlayer &player = ducks_info[action.duck_id];
    player.actualice_status(action.command);
}

void GameLoop::update_game_status() {
    int i = 0;
    for (auto &duck : ducks_info) {
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

void GameLoop::push_responce() {
    snaps_queue_list.send_to_every(game_status);
}

GameLoop::~GameLoop(){}

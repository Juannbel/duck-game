#include "game_loop.h"
#include <cmath>
#include <chrono>
using namespace std::chrono;

const milliseconds RATE(10);
const uint32_t JUMP_IT = 10;
const uint32_t DUCK_WIDE = 10;
const uint32_t DUCK_HIGH = 25;
const float DUCK_SPEED = 4;
const float FALL_SPEED = 4;


struct Colition rectangles_colition(const struct Rectangle &r1, const struct Rectangle &r2){
    struct Colition colitions = {false, false};
    if ((r1.y <= r2.y && r1.y+r1.high >= r2.y) || (r1.y <= r2.y+r2.high && r1.y+r1.high >= r2.y+r2.high)) {
        if((r1.x >= r2.x && r1.x <= r2.x+r2.wide) || (r1.x+r1.wide >= r2.x && r1.x+r1.wide <= r2.x+r2.wide)){
            colitions.vertical_colition = true;
            colitions.horizontal_colition = true;
            if (r1.y+r1.high == r2.y || r1.y == r2.y+r2.high) {
                colitions.horizontal_colition = false;
            }
            return colitions;
        }
    }
    return colitions;
}

struct Colition GameLoop::check_near_blocks_colition(struct Rectangle &duck, uint32_t new_x, uint32_t new_y) {
    uint32_t row_index = duck.y/10;
    uint32_t i = (row_index < 10) ?  0 : row_index-10;
    uint32_t end_i = (row_index+10 > map_info.rows) ? map_info.rows : row_index+10;
    
    uint32_t column_index = duck.x/10;
    uint32_t j = (column_index < 10) ?  0 : column_index-10;
    uint32_t end_j = (column_index+10 > map_info.columns) ? map_info.columns : column_index+10;
    struct Rectangle final_rec = {new_x, new_y, duck.wide, duck.high};
    struct Colition colitions = {false, false};
    uint32_t count_row = 0;
    for (auto &vec : map_info.blocks) {
        ++count_row;
        if (count_row < i || count_row > end_i) {
            continue;
        }
        uint32_t count_col = 0;
        for (auto &block : vec) {
            ++count_col;
            if (count_col < j || count_col > end_j) {
                continue;
            }
            if (block.type == 0) {
                continue;
            }
            struct Rectangle &block_rec = block.rectangle;
            struct Colition aux_colition = rectangles_colition(final_rec, block_rec);
            if (aux_colition.horizontal_colition){
                final_rec.x = duck.x;
                colitions.horizontal_colition = true;
                bool vertical_colition = rectangles_colition(final_rec, block_rec).vertical_colition;
                if(vertical_colition) {
                    if (new_y > duck.y && new_y+duck.high > block_rec.y && duck.y < block_rec.y) {
                        final_rec.y = block_rec.y-duck.high;
                    } else if (new_y < block_rec.y+block_rec.high && duck.y > block_rec.y) {
                        final_rec.y = duck.y;
                    }
                    colitions.vertical_colition = true;
                }
            }else if (aux_colition.vertical_colition) {
                colitions.vertical_colition = true;
            }
        }
        
    }
    duck.x = final_rec.x;
    duck.y = final_rec.y;
    return colitions;
}

void GameLoop::load_map() {
    for (uint32_t i = 0; i < 30; i++){
        std::vector<struct Block> blocks(30);
        for (struct Block &block : blocks) {
            block.type = 0;
            block.rectangle = {0, 0, 0, 0};
            
        }
        // Agrego paredes ficticias al rededor de todo el mapa
        blocks[0].type = 1; 
        blocks[0].rectangle = {0, i*10, 10, 10};
        blocks[29].type = 1;
        blocks[29].rectangle = {300, i*10, 10, 10};
        
        map_info.blocks.push_back(blocks);
    }
    std::vector<struct Block> &floor = map_info.blocks[15];
    for (uint32_t i = 0; i < 30; i++) { // Agrego pisos ficticias desde (0,150) a (300, 150)
        floor[i].type = 1;
        floor[i].rectangle = {i*10, 150, 10, 10};
    }
    std::vector<struct Block> &floor2 = map_info.blocks[8];
    for (uint32_t i = 0; i < 30; i++) { // Agrego pisos ficticias desde (0,80) a (300, 80)
        floor2[i].type = 1;
        floor2[i].rectangle = {i*10, 80, 10, 10};
    }
    map_info.rows = 30;
    map_info.columns = 30;
}

void GameLoop::verify_spawn() {
    // Si paso el tiempo suficiente, genera algo en ese spawn
}

void GameLoop::push_responce() {
    snaps_queue_list.send_to_every(ducks_info);
}

void GameLoop::process_action(struct action& action) {
    struct Duck &duck = ducks_info.ducks[action.duck_id];
    switch (action.command)
    {
    case StartMovingRight:
        if (!duck.is_lying){
            duck.facing_right = true;
            duck.is_running = true;
        }
        break;
    case StopMovingRight:
        duck.facing_right = true;
        duck.is_running = false;
        break;
    case StartMovingLeft:
        if (!duck.is_lying){
            duck.facing_right = false;
            duck.is_running = true;
        }
        break;
    case StopMovingLeft:
        duck.facing_right = false;
        duck.is_running = false;
        break;
    case StartShooting:
        duck.is_shooting = true;
        break;
    case StopShooting:
        duck.is_shooting = false;
        break;
    case StartFlapping:
        duck.is_flapping = duck.is_falling;
        break;
    case StopFlapping:
        duck.is_flapping = false;
        break;
    case LayDown:
        duck.is_lying = (duck.is_falling || duck.is_jumping) ? false : true;
        duck.is_running = duck.is_lying ? false : duck.is_running;
        break;
    case StandUp:
        duck.is_lying = false;
        break;
    case Jump:
        if (duck.is_falling) {
            break;
        }
        duck.it_jumping = duck.is_jumping ? duck.it_jumping : 1;
        duck.is_jumping = true;
        break;
    default:
        break;
    }
}

void GameLoop::update_game_status() {
    for (auto &duck : ducks_info.ducks) {
        struct Rectangle duck_rec = {duck.x, duck.y, DUCK_WIDE, DUCK_HIGH}; 
        uint32_t new_x = duck.x;
        uint32_t new_y = duck.y;
        if (duck.is_running) {
            float move_x = 0;
            if (duck.is_jumping){
                move_x = DUCK_SPEED*0.7;
            }else if(duck.is_falling){
                move_x = duck.is_flapping ? DUCK_SPEED*0.4 : DUCK_SPEED*0.7;
            }
            else if (!duck.is_lying){
                move_x = DUCK_SPEED;
            }
            new_x = (duck.facing_right) ? new_x+move_x : new_x-move_x;
        }
        if (duck.is_falling){
            float move_y = duck.is_flapping ? FALL_SPEED/2 : FALL_SPEED;
            new_y+=move_y;
        }
        if(duck.is_jumping){
            float move_y = FALL_SPEED;
            ++duck.it_jumping;
            new_y = duck.y-move_y;
        }
        struct Colition colition = check_near_blocks_colition(duck_rec, new_x, new_y);
        duck.x = duck_rec.x;
        duck.y = duck_rec.y;
        if (colition.vertical_colition && duck.is_falling){
            duck.is_falling = false;
        }else if(colition.vertical_colition && duck.is_jumping){
            duck.is_falling = true;
            duck.is_jumping = false;
            duck.it_jumping = 0;
        }
        else if(!colition.vertical_colition && !duck.is_falling){
            duck.is_falling = true;
        }
        if (duck.it_jumping > JUMP_IT){
            duck.is_jumping = false;
            duck.is_falling = true;
            duck.it_jumping = 0;
        }
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


GameLoop::GameLoop(Queue<struct action>& game_queue, QueueListMonitor& queue_list, uint8_t players_quantity) : 
    actions_queue(game_queue), snaps_queue_list(queue_list) {
    ducks_info.players_quantity = players_quantity;
    uint8_t i = 0;
    for(auto &duck : ducks_info.ducks){
        duck = {i, 100, 0, 0, None, false, false, false, false, false, false, false, false, false, false, 15, 150-DUCK_HIGH};
        duck.duck_hp = 100;
        ++i;
    }
    load_map();
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

GameLoop::~GameLoop(){}
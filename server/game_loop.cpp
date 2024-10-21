#include "game_loop.h"
#include <cmath>
#include <chrono>
using namespace std::chrono;

const milliseconds RATE(10);
const uint32_t JUMP_IT = 10;
const float DUCK_SPEED = 4;
const float FALL_SPEED = 4;
const int32_t NEAR_CELLS = 3;

GameLoop::GameLoop(Queue<struct action>& game_queue, QueueListMonitor& queue_list, uint8_t players_quantity) : 
    actions_queue(game_queue), snaps_queue_list(queue_list) , ducks_info(), map_info(){
    ducks_info.players_quantity = players_quantity;
    uint8_t i = 0;
    for(auto &duck : ducks_info.ducks){
        if (i < players_quantity) {
            duck.duck_hp = 100;
            duck.x = 20; 
            duck.y = 160-DUCK_HITBOX_HEIGHT; // Pongo un spawn a mano
        }else{
            duck.is_dead = true;
        }
        duck.duck_id = i;
        
        ++i;
    }
    load_map();
}

void GameLoop::load_map() {
    for (int32_t i = 0; i < MAP_HEIGHT_BLOCKS; i++){
        std::vector<struct Block> blocks(MAP_WIDTH_BLOCKS);
        for (struct Block &block : blocks) {
            block.type = 0;
            block.rectangle = {0, 0, 0, 0};
            
        }
        // Agrego paredes ficticias al rededor de todo el mapa
        blocks[0].type = 1; 
        blocks[0].rectangle = {0, i*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
        blocks[MAP_WIDTH_BLOCKS-1].type = 1;
        blocks[MAP_WIDTH_BLOCKS-1].rectangle = {MAP_WIDTH_PIXELS-BLOCK_SIZE, i*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
        
        map_info.blocks.push_back(blocks);
    }
    int floor_index = 10;
    std::vector<struct Block> &floor = map_info.blocks[floor_index];
    for (int32_t i = 0; i < MAP_WIDTH_BLOCKS; i++) { // Agrego pisos ficticias desde (0,160) a (560, 160)
        floor[i].type = 1;
        floor[i].rectangle = {i*BLOCK_SIZE, floor_index*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
    }
    floor_index = 5;
    std::vector<struct Block> &floor2 = map_info.blocks[floor_index];
    for (int32_t i = 0; i < MAP_WIDTH_BLOCKS; i++) { // Agrego pisos ficticias desde (0,80) a (525, 80)
        floor2[i].type = 1;
        floor2[i].rectangle = {i*BLOCK_SIZE, floor_index*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
    }
    map_info.rows = MAP_HEIGHT_BLOCKS;
    map_info.columns = MAP_WIDTH_BLOCKS;
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
    struct Duck &duck = ducks_info.ducks[action.duck_id];
    switch (action.command)
    {
    case StartMovingRight:
        if (!duck.is_laying) {
            duck.facing_right = true;
            duck.is_running = true;
        }
        break;
    case StartMovingLeft:
        if (!duck.is_laying){
            duck.facing_right = false;
            duck.is_running = true;
        }
        break;
    case StopMoving:
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
        duck.is_laying = (duck.is_falling || duck.is_jumping) ? false : true;
        duck.is_running = duck.is_laying ? false : duck.is_running;
        break;
    case StandUp:
        duck.is_laying = false;
        break;
    case Jump:
        if (duck.is_falling) {
            break;
        }
        duck.it_jumping = duck.is_jumping ? duck.it_jumping : 1;
        duck.is_jumping = true;
        break;
    case PickUp:
        // Agarrar si hay algo cerca
        break;
    case DropGun:
        duck.gun = None;
        duck.ammo = 0;
        break;
    case DropArmor:
        duck.armor_equiped = false;
        break;
    case DropHelmet:
        duck.helment_equiped = false;
        break;
    default:
        break;
    }
}

void GameLoop::update_game_status() {
    for (auto &duck : ducks_info.ducks) {
        if (duck.is_dead) {
            continue;
        }
        move_duck(duck);
        // disparar
    }
    // Actualizar la posicion de las balas y vida de los patos si les pegan
}

void update_duck_status(struct Duck &duck, struct Collision &collision){
    if (collision.vertical_collision && duck.is_falling){
        duck.is_falling = false;
    }else if(collision.vertical_collision && duck.is_jumping){
        duck.is_falling = true;
        duck.is_jumping = false;
        duck.it_jumping = 0;
    }
    else if(!collision.vertical_collision && !duck.is_falling){
        duck.is_falling = true;
    }
    if (duck.it_jumping > JUMP_IT){
        duck.is_jumping = false;
        duck.is_falling = true;
        duck.it_jumping = 0;
    }
}

void GameLoop::move_duck(struct Duck &duck){
    struct Rectangle duck_rec = {duck.x, duck.y, DUCK_HITBOX_WIDTH, DUCK_HITBOX_HEIGHT}; 
    int32_t new_x = duck.x;
    int32_t new_y = duck.y;
    if (duck.is_running) {
        float move_x = 0;
        if (duck.is_jumping){
            move_x = DUCK_SPEED*0.7;
        }else if(duck.is_falling){
            move_x = duck.is_flapping ? DUCK_SPEED*0.4 : DUCK_SPEED*0.7;
        }
        else if (!duck.is_laying){
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
    struct Collision collision = check_near_blocks_collision(duck_rec, new_x, new_y);
    duck.x = duck_rec.x;
    duck.y = duck_rec.y;
    update_duck_status(duck, collision);
}

struct Collision GameLoop::check_near_blocks_collision(struct Rectangle &duck, int32_t new_x, int32_t new_y) {
    int32_t row_index = duck.y/BLOCK_SIZE;
    int32_t i = (row_index < NEAR_CELLS) ?  0 : row_index-NEAR_CELLS;
    int32_t end_i = (row_index+NEAR_CELLS > map_info.rows) ? map_info.rows : row_index+NEAR_CELLS;
    
    int32_t column_index = duck.x/BLOCK_SIZE;
    int32_t j = (column_index < NEAR_CELLS) ?  0 : column_index-NEAR_CELLS;
    int32_t end_j = (column_index+NEAR_CELLS > map_info.columns) ? map_info.columns : column_index+NEAR_CELLS;
    struct Rectangle final_rec = {new_x, new_y, duck.width, duck.height};
    struct Collision collision = {false, false};
    int32_t count_row = 0;
    for (auto &vec : map_info.blocks) {
        ++count_row;
        if (count_row < i || count_row > end_i) {
            continue;
        }
        int32_t count_col = 0;
        for (auto &block : vec) {
            ++count_col;
            if (count_col < j || count_col > end_j) {
                continue;
            }
            if (block.type == 0) {
                continue;
            }
            struct Rectangle &block_rec = block.rectangle;
            struct Collision aux_collision = rectangles_collision(final_rec, block_rec);
            if (aux_collision.horizontal_collision){
                final_rec.x = duck.x;
                collision.horizontal_collision = true;
                bool vertical_collision = rectangles_collision(final_rec, block_rec).vertical_collision;
                if(vertical_collision) {
                    if (new_y > duck.y && new_y+duck.height > block_rec.y && duck.y < block_rec.y) {
                        final_rec.y = block_rec.y-duck.height;
                    } else if (new_y < block_rec.y+block_rec.height && duck.y > block_rec.y) {
                        final_rec.y = duck.y;
                    }
                    collision.vertical_collision = true;
                }
            }else if (aux_collision.vertical_collision) {
                collision.vertical_collision = true;
            }
        }
        
    }
    duck.x = final_rec.x;
    duck.y = final_rec.y;
    return collision;
}

struct Collision GameLoop::rectangles_collision(const struct Rectangle &r1, const struct Rectangle &r2){
    struct Collision collision = {false, false};
    if ((r1.y <= r2.y && r1.y+r1.height >= r2.y) || (r1.y <= r2.y+r2.height && r1.y+r1.height >= r2.y+r2.height)) {
        if((r1.x >= r2.x && r1.x <= r2.x+r2.width) || (r1.x+r1.width >= r2.x && r1.x+r1.width <= r2.x+r2.width)){
            collision.vertical_collision = true;
            collision.horizontal_collision = true;
            if (r1.y+r1.height == r2.y || r1.y == r2.y+r2.height) {
                collision.horizontal_collision = false;
            }
            return collision;
        }
    }
    return collision;
}

void GameLoop::verify_spawn() {
    // Si paso el tiempo suficiente, genera algo en ese spawn
}

void GameLoop::push_responce() {
    snaps_queue_list.send_to_every(ducks_info);
}

GameLoop::~GameLoop(){}
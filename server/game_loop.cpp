#include "game_loop.h"
#include <cmath>
#include <chrono>
using namespace std::chrono;

const milliseconds RATE(10);
const uint32_t JUMP_IT = 40;
const float DUCK_SPEED = 10;
const float FALL_SPEED = 3;

void GameLoop::load_map() {
    // cargar las estructuras de mapas
    // cargar puntos de spawn
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
        if (duck.is_running) {
            float move_x = 0;
            if (duck.is_jumping){
                move_x = DUCK_SPEED*0.707;
            }else if(duck.is_falling){
                move_x = duck.is_flapping ? DUCK_SPEED*0.4 : DUCK_SPEED*0.707;
            }
            else if (!duck.is_lying){
                move_x = DUCK_SPEED;
            }
            if (duck.facing_right && duck.x+move_x > 200) { // "Verificación" de colisiones
                duck.x = 200;
            }
            else if(!duck.facing_right && duck.x < move_x){
                duck.x = 0;
            }else{ 
                duck.x = (duck.facing_right) ? duck.x+move_x : duck.x-move_x;
            }
        }
        if (duck.is_falling){
            float move_y = duck.is_flapping ? FALL_SPEED/2 : FALL_SPEED;
            if (duck.y+move_y > 150) { // Verificacion de colisiones
                duck.is_falling = false;
                duck.is_flapping = false;
                duck.y = 150;
            }else{duck.y+=move_y;}
        }
        if(duck.is_jumping){
            float move_y = FALL_SPEED;
            duck.it_jumping+=4;
            if (duck.it_jumping > JUMP_IT){
                duck.is_jumping = false;
                duck.is_falling = true;
            }
            else {
                duck.y = duck.y-move_y;
            }
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
        duck = {i, 100, 0, 0, None, false, false, false, false, false, false, false, false, false, false, 0, 150};
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
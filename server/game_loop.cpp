#include "game_loop.h"
#include <cmath>
#include <chrono>
using namespace std::chrono;

const milliseconds rate(10);

void GameLoop::load_map() {
    // cargar las estructuras de mapas
    // cargar puntos de spawn
}

void GameLoop::verify_spawn() {
    // Si paso el tiempo suficiente, genera algo en ese spawn
}

void GameLoop::push_responce() {
    // broadcast status del juego actual
}

void GameLoop::process_action(struct action& action) {
    // actualizar estados
    // 
}

void GameLoop::pop_and_process_all() {
    struct action action;
    while (actions_queue.try_pop(action)){
        process_action(action);
        push_responce();
    }
    verify_spawn();
}


GameLoop::GameLoop(Queue<struct action>& game_queue, QueueListMonitor& queue_list) : 
    actions_queue(game_queue), snaps_queue_list(queue_list) {
    load_map();
}

void GameLoop::run(){
    auto t1 = high_resolution_clock::now();
    uint it = 0;
    while(_keep_running){
        pop_and_process_all();
        auto t2 = high_resolution_clock::now();
        milliseconds duration = duration_cast<milliseconds>(t2 - t1);
        milliseconds rest = rate - duration;
        if (rate < duration){
            milliseconds behind = duration - rest; 
            milliseconds lost = behind - behind % rate;
            t1 += lost;
            it += floor(lost / rate);  
        }else {
            std::this_thread::sleep_for(rest);
        }
        t1 += rate;
        ++it;
        std::cout << "Iteración " << it << "duró:" << rest.count() << " milisegundos" << "\n";
    }
}

GameLoop::~GameLoop(){}
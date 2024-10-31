#include "game.h"
#include <cstdint>
#include <cstdio>

Game::Game(const int id): gameloop(gameloop_q, sv_msg_queues), id(id) {
    open = true;
    cant_players = 0;
}

Queue<action>& Game::get_gameloop_queue() {
    return gameloop_q;
}

//uint8_t Game::get_duck_id() {
//    return cant_players;
//}

uint8_t Game::add_player(int player_id, Queue<Snapshot>& player_sender_queue) {
    if (!open || cant_players == MAX_DUCKS) {
        std::cout << "No entran mas jugadores" << std::endl;
        return -1;
    }

    sv_msg_queues.add_element(&player_sender_queue);

    uint8_t game_duck_id = gameloop.add_player();
    player_to_duck_id[player_id] = game_duck_id;
    cant_players++;
    if (cant_players == MAX_DUCKS) {
        open = false;
    }
    return game_duck_id; 
}

void Game::start() {
    gameloop.start();
    open = false;
}

// TODO: caso en que el jugador se desconecta en medio de la partida
// Game::delete_player(ServerClient* player) {}

Game::~Game() {
    gameloop.stop();
    gameloop.join();    
}

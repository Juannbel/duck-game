#include "game.h"

#include <cstdint>
#include <cstdio>
#include "common/lobby.h"

Game::Game(const int id): gameloop(gameloop_q, sv_msg_queues), id(id) {
    open = true;
    cant_players = 0;
}

Queue<action>& Game::get_gameloop_queue() { return gameloop_q; }

uint8_t Game::add_player(int id_player, Queue<Snapshot>& player_sender_queue) {
    if (!open || cant_players == MAX_DUCKS) {
        std::cout << "No entran mas jugadores" << std::endl;
        return INVALID_DUCK_ID;
    }

    sv_msg_queues.add_element(&player_sender_queue);

    const uint8_t game_duck_id = gameloop.add_player();
    player_to_duck_id[id_player] = game_duck_id;
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

void Game::delete_player(const int id_player) {
    // ver como acceder a la queue del player para sacarla de la lista (tiene que ser con playerId)
    // Queue<Snapshot>* queue = player.get_sender_queue();
    // sv_msg_queues.remove_element(queue);
    const int duck_id = player_to_duck_id[id_player];
    gameloop.delete_duck(duck_id);
}

Game::~Game() {
    gameloop.stop();
    gameloop.join();
}

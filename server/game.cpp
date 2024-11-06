#include "game.h"

#include <cstdint>
#include <cstdio>
#include <utility>

#include "common/lobby.h"

Game::Game(const int id): gameloop(gameloop_q, sv_msg_queues), id(id) {
    open = true;
    cant_players = 0;
}

Queue<action>& Game::get_gameloop_queue() { return gameloop_q; }

GameInfo Game::add_player(int id_player, Queue<Snapshot>& player_sender_queue, const int num_players) {
    GameInfo game_info = {INVALID_GAME_ID, INVALID_DUCK_ID, INVALID_DUCK_ID};
    if (!open || cant_players + num_players > MAX_DUCKS) {
        return game_info;
    }

    game_info.game_id = id;
    sv_msg_queues.add_element(&player_sender_queue);

    std::pair<uint8_t, uint8_t> duck_ids = {INVALID_DUCK_ID, INVALID_DUCK_ID};
    duck_ids.first = gameloop.add_player();
    game_info.duck_id_1 = duck_ids.first;

    if (num_players == 2) {
        duck_ids.second = gameloop.add_player();
        game_info.duck_id_2 = duck_ids.second;
    }

    player_to_duck_ids[id_player] = duck_ids;
    cant_players += num_players;

    if (cant_players == MAX_DUCKS) {
        open = false;
    }
    return game_info;
}

void Game::start() {
    gameloop.start();
    open = false;
}

void Game::delete_player(const int id_player) {
    // ver como acceder a la queue del player para sacarla de la lista (tiene que ser con playerId)
    // Queue<Snapshot>* queue = player.get_sender_queue();
    // sv_msg_queues.remove_element(queue);
    const std::pair<uint8_t, uint8_t> duck_ids = player_to_duck_ids[id_player];
    gameloop.delete_duck(duck_ids.first);
    if (duck_ids.second != INVALID_DUCK_ID) {
        gameloop.delete_duck(duck_ids.second);
    }
}

Game::~Game() {
    gameloop.stop();
    gameloop.join();
}

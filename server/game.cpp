#include "game.h"

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "common/lobby.h"

Game::Game(const int id, const std::string& creator, const int owner_id):
        gameloop(gameloop_q, sv_msg_queues),
        cant_players(0),
        creator(creator),
        owner_id(owner_id),
        owner_queue(nullptr),
        open(true),
        game_id(id) {
    gameloop.start();
}

Queue<action>& Game::get_gameloop_queue() { return gameloop_q; }

GameInfo Game::add_player(int player_id, Queue<Snapshot>& player_sender_queue,
                          const std::vector<std::string>& players_names) {

    GameInfo game_info = {INVALID_GAME_ID, INVALID_DUCK_ID, INVALID_DUCK_ID};

    if (!open || cant_players + players_names.size() > MAX_DUCKS) {
        return game_info;
    }

    game_info.game_id = game_id;
    if (player_id != owner_id)
        sv_msg_queues.add_element(&player_sender_queue, player_id);
    else
        owner_queue = &player_sender_queue;


    std::pair<uint8_t, uint8_t> duck_ids = {INVALID_DUCK_ID, INVALID_DUCK_ID};
    duck_ids.first = gameloop.add_player(players_names[0]);
    game_info.duck_id_1 = duck_ids.first;

    if (players_names.size() == 2) {
        duck_ids.second = gameloop.add_player(players_names[1]);
        game_info.duck_id_2 = duck_ids.second;
    }

    player_to_duck_ids[player_id] = duck_ids;
    cant_players += players_names.size();

    if (cant_players == MAX_DUCKS) {
        open = false;
    }
    return game_info;
}

void Game::start() {
    assert(owner_queue != nullptr);
    sv_msg_queues.add_element(owner_queue, owner_id);
    gameloop.start_game();
    open = false;
}

bool Game::delete_player(const int id_player) {
    Queue<Snapshot>* player_sender_q = sv_msg_queues.remove_element(id_player);
    if (player_sender_q) {
        player_sender_q->close();
    }
    // indica si el owner se fue antes de que el juego arranque
    bool owner_left = false;
    if (!gameloop.is_initialized() &&
        id_player == owner_id) {  // si el dueño se va, se cierra el juego
        assert(owner_queue != nullptr);
        owner_queue->close();
        owner_left = true;
    }
    const std::pair<uint8_t, uint8_t> duck_ids = player_to_duck_ids[id_player];
    gameloop.delete_duck(duck_ids.first);
    cant_players--;
    if (duck_ids.second != INVALID_DUCK_ID) {
        if (gameloop.is_alive() || (!gameloop.is_initialized() && !gameloop.is_alive())) {
            gameloop.delete_duck(duck_ids.second);
            cant_players--;
        }
    }
    if (cant_players < MAX_DUCKS && !gameloop.is_initialized() && !owner_left) {
        open = true;
    }
    return cant_players == 0 || owner_left;
}

Game::~Game() {
    gameloop.stop();            // siempre se puede hacer stop por el wrapper
    if (gameloop.joinable()) {  // NO siempre se puede hacer join
        gameloop.join();
    }

    for (auto& [player_id, _] : player_to_duck_ids) {
        Queue<Snapshot>* sender_q = sv_msg_queues.remove_element(player_id);
        if (sender_q)
            sender_q->close();
    }
}

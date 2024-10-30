#include "game.h"
#include <cstdio>
#include "common/map_dto.h"

Game::Game(const int id): map(map_loader.loadMap(SERVER_DATA_PATH "/map1.yaml")), gameloop(gameloop_q, sv_msg_queues, map), id(id) {
    open = true;
    cant_players = 0;
    match_info.map = map;
}

Queue<action>& Game::get_gameloop_queue() {
    return gameloop_q;
}

MatchInfo Game::get_match_info() {
    return match_info;
}

void Game::add_player(int player_id, Queue<Snapshot>& player_sender_queue) {
    if (!open || cant_players == MAX_DUCKS) {
        // que hago aca? exception?
        return;
    }

    sv_msg_queues.add_element(&player_sender_queue);

    int game_duck_id = gameloop.add_player(player_id);
    match_info.duck_id = game_duck_id;
    cant_players++;
    if (cant_players == MAX_DUCKS) {
        open = false;
    }
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

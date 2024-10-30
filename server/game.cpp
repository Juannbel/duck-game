#include "game.h"
// TODO: Resolver inicio de gameloop, se dibujan todos los patos y segun el duckid se manejan distintos (si entra un cliente con duckid 5 funciona? cambiar eso)
Game::Game(const int id): map(map_loader.loadMap(SERVER_DATA_PATH "/map1.yaml")), gameloop(gameloop_q, sv_msg_queues, map, MAX_DUCKS), id(id) {}

Queue<action>& Game::get_gameloop_queue() {
    return gameloop_q;
}

MatchInfo Game::get_match_info() {
    return MatchInfo(0, map);
}
// TODO: usar los player_id, para borrar despues
void Game::add_player(int player_id, Queue<Snapshot>& player_sender_queue) {
    if (!open || cant_players == MAX_DUCKS) {
        // que hago aca? exception?
        return;
    }

    // Todo: sacar esto, esta solo para evitar warning
    player_id++;

    sv_msg_queues.add_element(&player_sender_queue);

    cant_players++;
    if (cant_players == MAX_DUCKS) {
        open = false;
    }
    return;
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

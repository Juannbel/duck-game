#include "game.h"
#include "common/map_dto.h"

Game::Game(int id): map(map_loader.loadMap(SERVER_DATA_PATH "/map1.yaml")), gameloop(gameloop_q, sv_msg_queues, map, MAX_DUCKS), 
    id(id) {}


Queue<struct action>& Game::get_gameloop_queue() {
    return gameloop_q;
}

void Game::add_player(ServerClient* player) {
    sv_msg_queues.add_element(&(player->get_sender_queue()));
    player->set_gameloop_queue(&gameloop_q);
    player->send_game_info(MatchInfo(cant_players, map));
    cant_players++;
    if (cant_players == MAX_DUCKS) {
        gameloop.start();
        open = false;
    }
}


// TODO: caso en que el jugador se desconecta en medio de la partida
// Game::delete_player(ServerClient* player) {}

Game::~Game() {
    gameloop.stop();
    gameloop.join();    
}

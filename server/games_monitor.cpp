#include "games_monitor.h"

#define CREATE 1
#define JOIN 2

// TODO: el mutex no es necesario??, El acceptor es el unico que va a agregar players
// y el acceptor es unico
void GamesMonitor::add_player(ServerClient* player) {
    int cmd = player->receive_cmd();
    if (cmd == CREATE) {
        Game* game = create_game();
        game->add_player(player);
    } else if (cmd == JOIN) {
        player->send_games_info(list_lobbies());
        // caso en que no se encuentren lobbies
        if (map_games.empty()) {
            add_player(player);
        }
        // esperar por el id del juego
        int id = player->receive_cmd();
        Game* game = map_games[id];
        game->add_player(player);
    } 
}

std::vector<int> GamesMonitor::list_lobbies() {
    std::vector<int> lobbies;
    for (auto& game: map_games) {
        if (game.second->is_open()) {
            lobbies.push_back(game.first);
        }
    }
    lobbies.push_back(-1);
    return lobbies;
}

Game* GamesMonitor::create_game() {
    Game* game = new Game(id);
    map_games.emplace(id, game);
    id++;
    return game;
}

//void GamesMonitor::delete_game(int id) {}
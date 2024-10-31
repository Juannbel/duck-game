#include "games_monitor.h"
#include <cstdint>
#include <utility>

#define CREATE 1
#define JOIN 2

// TODO: ver bien tema de mutex y accesos entre threads

int GamesMonitor::player_create_game(const int id_player, Queue<Snapshot>& player_sender_queue, uint8_t& duck_id) {
    //std::lock_guard<std::mutex> lck(m);
    Game* game = create_game();
    duck_id = game->add_player(id_player, player_sender_queue);
    return game->get_id();
}

uint8_t GamesMonitor::player_join_game(const int id_player, const int id_game, Queue<Snapshot>& player_sender_queue) {
    std::lock_guard<std::mutex> lck(m);
    Game* game = map_games[id_game];
    return game->add_player(id_player, player_sender_queue);
}

void GamesMonitor::start_game(int id_game) {
    std::lock_guard<std::mutex> lck(m);
    Game* game = map_games[id_game];
    game->start();
}

std::vector<int> GamesMonitor::list_lobbies() {
    std::lock_guard<std::mutex> lck(m);
    std::vector<int> lobbies;
    for (auto& game : map_games) {
        if (game.second->is_open()) {
            lobbies.push_back(game.first);
        }
    }
    lobbies.push_back(-1);
    return lobbies;
}

Game* GamesMonitor::create_game() {
    Game* game = new Game(id);
    std::lock_guard<std::mutex> lck(m);
    map_games.emplace(id, game);
    id++;
    return game;
}

Queue<action>* GamesMonitor::get_gameloop_q(const int id_game) {
    std::lock_guard<std::mutex> lck(m);
    Game* game = map_games[id_game];
    return &game->get_gameloop_queue();
}

GamesMonitor::~GamesMonitor() {
    std::lock_guard<std::mutex> lck(m);
    for (auto& game: map_games) {
        delete game.second;
    }
}

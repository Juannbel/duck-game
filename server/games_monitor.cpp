#include "games_monitor.h"

#include <cstdint>
#include <utility>

#include "common/lobby.h"

#define CREATE 1
#define JOIN 2

GameInfo GamesMonitor::player_create_game(const int id_player, Queue<Snapshot>& player_sender_queue,
                                 const int num_players) {
    Game* game = create_game();
    GameInfo game_info = game->add_player(id_player, player_sender_queue, num_players);
    return game_info;
}

GameInfo GamesMonitor::player_join_game(const int id_player, const int id_game,
                                       Queue<Snapshot>& player_sender_queue, const int num_players) {
    std::lock_guard<std::mutex> lck(m);
    if (map_games.find(id_game) == map_games.end()) {
        return {INVALID_GAME_ID, INVALID_DUCK_ID, INVALID_DUCK_ID};
    }
    Game* game = map_games[id_game];
    return game->add_player(id_player, player_sender_queue, num_players);
}

void GamesMonitor::start_game(int id_game) {
    std::lock_guard<std::mutex> lck(m);
    Game* game = map_games[id_game];
    game->start();
}

std::vector<int> GamesMonitor::list_lobbies() {
    std::lock_guard<std::mutex> lck(m);
    std::vector<int> lobbies;
    for (auto& game: map_games) {
        if (game.second->is_open()) {
            lobbies.push_back(game.first);
        }
    }
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

void GamesMonitor::remove_player(const int id_game, const int id_player) {
    std::lock_guard<std::mutex> lck(m);
    Game* game = map_games[id_game];
    game->delete_player(id_player);
}

GamesMonitor::~GamesMonitor() {
    std::lock_guard<std::mutex> lck(m);
    for (auto& game: map_games) {
        delete game.second;
    }
}

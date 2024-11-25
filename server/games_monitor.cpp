#include "games_monitor.h"

#include <string>
#include <utility>

#include "common/lobby.h"

#define CREATE 1
#define JOIN 2

GameInfo GamesMonitor::player_create_game(const int id_player, Queue<Snapshot>& player_sender_queue,
                                          const std::vector<std::string>& players_names) {
    Game* game = create_game(players_names[0], id_player);
    GameInfo game_info = game->add_player(id_player, player_sender_queue, players_names);
    return game_info;
}

GameInfo GamesMonitor::player_join_game(const int id_player, const int id_game,
                                        Queue<Snapshot>& player_sender_queue,
                                        const std::vector<std::string>& players_names) {
    std::lock_guard<std::mutex> lck(m);
    if (map_games.find(id_game) == map_games.end()) {
        return {INVALID_GAME_ID, INVALID_DUCK_ID, INVALID_DUCK_ID};
    }
    Game* game = map_games[id_game];
    return game->add_player(id_player, player_sender_queue, players_names);
}

void GamesMonitor::start_game(int id_game) {
    std::lock_guard<std::mutex> lck(m);
    Game* game = map_games[id_game];
    game->start();
}

std::vector<LobbyInfo> GamesMonitor::list_lobbies() {
    std::lock_guard<std::mutex> lck(m);
    std::vector<LobbyInfo> lobbies;
    for (auto& game: map_games) {
        Game* game_ptr = game.second;
        if (game_ptr && game_ptr->is_open()) {
            lobbies.push_back(game.second->get_info());
        }
    }
    return lobbies;
}

LobbyInfo GamesMonitor::get_lobby_info(int32_t id_game) {
    // esto no es atomic, asi que a priori si otro thread modifica el contendor, esto genera UB
    // -> tomar primero el lock
    if (map_games.find(id_game) == map_games.end()) {
        return LobbyInfo{};
    }
    std::lock_guard<std::mutex> lck(m);
    Game* game = map_games[id_game];
    return game->get_info();
}

Game* GamesMonitor::create_game(const std::string& creator_name, const int id_player) {
    // race condition
    // primero el lock, luego crean un game
    // tranquilamente podrían crear dos juegos con el mismo id

    // podrían haber utilizado smart pointers para evitar new/delete
    Game* game = new Game(id, creator_name, id_player);
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
    if (Game* game = map_games[id_game]) {
        if (game->delete_player(id_player)) {
            delete game;
            map_games.erase(id_game);
        }
    }
}

void GamesMonitor::delete_games() {
    std::lock_guard<std::mutex> lck(m);
    for (auto& game: map_games) {
        delete game.second;
    }
    map_games.clear();
}

GamesMonitor::~GamesMonitor() {}

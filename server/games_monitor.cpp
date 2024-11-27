#include "games_monitor.h"

#include <string>
#include <utility>

#include "common/lobby.h"

#define CREATE 1
#define JOIN 2

GameInfo GamesMonitor::player_create_game(const int id_player, Queue<Snapshot>& player_sender_queue,
                                          const std::vector<std::string>& players_names) {
    std::lock_guard<std::mutex> lck(m);
    const int id_game = create_game(players_names[0], id_player);
    const GameInfo game_info = map_games[id_game]->add_player(id_player, player_sender_queue, players_names);
    return game_info;
}

GameInfo GamesMonitor::player_join_game(const int id_player, const int id_game,
                                        Queue<Snapshot>& player_sender_queue,
                                        const std::vector<std::string>& players_names) {
    std::lock_guard<std::mutex> lck(m);
    if (!map_games.contains(id_game)) {
        return {INVALID_GAME_ID, INVALID_DUCK_ID, INVALID_DUCK_ID};
    }

    return map_games[id_game]->add_player(id_player, player_sender_queue, players_names);
}

void GamesMonitor::start_game(int id_game) {
    std::lock_guard<std::mutex> lck(m);
    map_games[id_game]->start();
}

std::vector<LobbyInfo> GamesMonitor::list_lobbies() {
    std::lock_guard<std::mutex> lck(m);
    std::vector<LobbyInfo> lobbies;
    for (auto& game: map_games) {
        Game* game_ptr = game.second.get();
        if (game_ptr && game_ptr->is_open()) {
            lobbies.push_back(game.second->get_info());
        }
    }
    return lobbies;
}

LobbyInfo GamesMonitor::get_lobby_info(const int32_t id_game) {
    std::lock_guard<std::mutex> lck(m);
    if (!map_games.contains(id_game)) {
        return LobbyInfo{};
    }
    return map_games[id_game]->get_info();
}

int GamesMonitor::create_game(const std::string& creator_name, const int id_player) {
    std::unique_ptr<Game> game = std::make_unique<Game>(id, creator_name, id_player);
    map_games.emplace(id, std::move(game));
    id++;
    return id - 1;
}

Queue<action>* GamesMonitor::get_gameloop_q(const int id_game) {
    std::lock_guard<std::mutex> lck(m);
    return &map_games[id_game]->get_gameloop_queue();
}

void GamesMonitor::remove_player(const int id_game, const int id_player) {
    std::lock_guard<std::mutex> lck(m);
    if (Game* game = map_games[id_game].get()) {
        if (game->delete_player(id_player)) {
            map_games.erase(id_game);
        }
    }
}

void GamesMonitor::delete_games() {
    std::lock_guard<std::mutex> lck(m);
    map_games.clear();
}

GamesMonitor::~GamesMonitor() {}

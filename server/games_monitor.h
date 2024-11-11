#ifndef GAMES_MONITOR_H
#define GAMES_MONITOR_H

#include <map>
#include <mutex>
#include <string>
#include <vector>

#include "common/lobby.h"

#include "game.h"

class Game;

class GamesMonitor {
private:
    std::mutex m;
    std::map<int, Game*> map_games;
    int id = 0;

public:
    GameInfo player_create_game(const int id_player, Queue<Snapshot>& player_sender_queue,
                                const std::vector<std::string>& players_names);

    GameInfo player_join_game(int id_player, int id_game, Queue<Snapshot>& player_sender_queue,
                              const std::vector<std::string>& players_names);

    void start_game(int id);

    Game* create_game(const std::string& creator_name);

    void delete_game(int id);

    std::vector<LobbyInfo> list_lobbies();

    Queue<action>* get_gameloop_q(int id_game);

    void remove_player(int id_game, int id_player);

    ~GamesMonitor();
};

#endif

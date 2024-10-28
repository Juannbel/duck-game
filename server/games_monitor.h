#ifndef GAMES_MONITOR_H
#define GAMES_MONITOR_H

#include <map>
#include <mutex>
#include "server_client.h"
#include "game.h"

class GamesMonitor {
private:
private:
    std::mutex m;
    std::map<int, Game*> map_games;
    int id = 0;
public:
    void add_player(ServerClient* player);

    Game* create_game();

    void delete_game(int id);

    std::vector<int> list_lobbies() const;

    ~GamesMonitor();
};

#endif
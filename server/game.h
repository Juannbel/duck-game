#ifndef GAME_H
#define GAME_H

#include "../common/blocking_queue.h"
#include "server/maps/yaml.h"
#include "server/server_client.h"
#include "game_loop.h"

class Game {
private:
    YAMLLoader map_loader;
    Map map;
    QueueListMonitor sv_msg_queues;
    Queue<struct action> gameloop_q;
    GameLoop gameloop;
    int cant_players = 0;
    bool open = true;
    int id;
public:
    Game(int id);

    Queue<struct action>& get_gameloop_queue();

    void add_player(ServerClient* player);

    bool is_open() { return open; }
    
    int get_id() { return id; }

    ~Game();
};

#endif
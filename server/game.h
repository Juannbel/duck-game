#ifndef GAME_H
#define GAME_H

#include "../common/blocking_queue.h"
#include "server/maps/yaml.h"
#include "common/map_dto.h"
#include "game_loop.h"

class Game {
private:
    YAMLLoader map_loader;
    Map map;
    QueueListMonitor sv_msg_queues;
    Queue<action> gameloop_q;
    GameLoop gameloop;
    MatchInfo match_info;
    int cant_players = 0;
    bool open = true;
    int id;
public:
    Game(int id);

    Queue<action>& get_gameloop_queue();

    MatchInfo get_match_info();

    void start();

    void add_player(int player_id, Queue<Snapshot>& player_sender_queue);

    bool is_open() { return open; }
    
    int get_id() { return id; }

    ~Game();
};

#endif
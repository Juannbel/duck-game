#ifndef GAME_H
#define GAME_H

#include <cstdint>
#include <map>

#include "../common/blocking_queue.h"

#include "game_loop.h"

class Game {
private:
    QueueListMonitor sv_msg_queues;
    Queue<action> gameloop_q;
    GameLoop gameloop;
    int cant_players = 0;
    std::map<int, uint8_t> player_to_duck_id;
    bool open = true;
    int id;

public:
    explicit Game(int id);

    Queue<action>& get_gameloop_queue();

    void start();

    uint8_t add_player(int player_id, Queue<Snapshot>& player_sender_queue);

    void delete_player(int id_player);

    bool is_open() { return open; }

    int get_id() { return id; }

    ~Game();
};

#endif

#ifndef GAME_H
#define GAME_H

#include <cstdint>
#include <map>
#include <utility>

#include "../common/blocking_queue.h"
#include "common/lobby.h"

#include "game_loop.h"

class Game {
private:
    QueueListMonitor sv_msg_queues;
    Queue<action> gameloop_q;
    GameLoop gameloop;
    int cant_players = 0;
    std::map<int, std::pair<uint8_t, uint8_t>> player_to_duck_ids;
    bool open = true;
    int id;

public:
    explicit Game(int id);

    Queue<action>& get_gameloop_queue();

    void start();

    GameInfo add_player(int player_id, Queue<Snapshot>& player_sender_queue, const int num_players);

    void delete_player(const int id_player);

    bool is_open() { return open; }

    int get_id() { return id; }

    ~Game();
};

#endif

#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#include <cstdint>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include <sys/types.h>

#include "common/blocking_queue.h"
#include "common/commands.h"
#include "common/map_loader.h"
#include "common/shared_constants.h"
#include "common/snapshot.h"
#include "common/thread.h"
#include "game/game_operator.h"

#include "list_monitor.h"

class GameLoop: public Thread {
private:
    Queue<struct action>& actions_queue;
    QueueListMonitor& snaps_queue_list;
    GameOperator game_operator;
    uint8_t round_number;
    bool round_finished;
    bool game_finished;
    std::map<uint8_t, uint8_t> winners_id_count;
    MapLoader map_loader;
    std::vector<std::string> paths_to_maps;
    Map curr_map;
    std::vector<std::pair<uint8_t, std::string>> ducks_info;

    void initialice_new_round();

    void initial_snapshot();

    void pop_and_process_all();

    void create_and_push_snapshot(auto& t1, uint& it);

    void add_rounds_won(Snapshot& snapshot);

    void push_responce(const Snapshot& actual_status);

    void check_for_winner(const Snapshot&);

public:
    GameLoop(Queue<struct action>& game_queue, QueueListMonitor& queue_list);
    /*   This class is the game loop.
     *   Start a loop that pop an action of the game queue, process it and
     *   push the responce to all the queues in the queue_list.
     *   To stop it, close the game_queue and call function stop().
     */
    virtual void run() override;

    uint8_t add_player(const std::string& player_name);
    // uint8_t add_player();

    void delete_duck(uint8_t duck_id);

    ~GameLoop();

private:
    GameLoop(const GameLoop&) = delete;
    GameLoop& operator=(const GameLoop&) = delete;
    GameLoop(GameLoop&& other) = delete;
    GameLoop& operator=(GameLoop&& other) = delete;
};

#endif

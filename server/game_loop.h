#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#include "common/blocking_queue.h"
#include "common/commands.h"
#include "common/map_dto.h"
#include "common/shared_constants.h"
#include "common/snapshot.h"
#include "common/thread.h"

#include "action.h"
#include "duck_player.h"
#include "entitys_manager.h"
#include "list_monitor.h"

#include <map>

struct Spawn {
    int16_t x;
    int16_t y;
    uint16_t it_since_picked;
    bool picked;
};

class GameLoop: public Thread {
private:
    uint8_t players_quantity;
    Queue<struct action>& actions_queue;
    QueueListMonitor& snaps_queue_list;
    DuckPlayer ducks_info[MAX_DUCKS];
    Map map_blocks_info;
    std::vector<Spawn> spawns;


    EntityManager entity_manager;

    void pop_and_process_all();

    void process_action(struct action& action);

    void update_game_status();

    void verify_spawn();

    void push_responce();

public:
    GameLoop(Queue<struct action>& game_queue, QueueListMonitor& queue_list, Map& map_dto,
             uint8_t players_quantity);
    /*   This class is the game loop.
     *   Start a loop that pop an action of the game queue, process it and
     *   push the responce to all the queues in the queue_list.
     *   To stop it, close the game_queue and call function stop().
     */
    virtual void run() override;

    ~GameLoop();

private:
    GameLoop(const GameLoop&) = delete;
    GameLoop& operator=(const GameLoop&) = delete;
    GameLoop(GameLoop&& other) = delete;
    GameLoop& operator=(GameLoop&& other) = delete;
};

#endif

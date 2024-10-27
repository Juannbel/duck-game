#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#include "common/blocking_queue.h"
#include "common/commands.h"
#include "common/map_dto.h"
#include "common/shared_constants.h"
#include "common/snapshot.h"
#include "common/thread.h"

#include "action.h"
#include "game/duck_player.h"
#include "game/entitys_manager.h"
#include "list_monitor.h"

#include <map>

class GameLoop: public Thread {
private:
    Queue<struct action>& actions_queue;
    QueueListMonitor& snaps_queue_list;
    EntityManager entity_manager;
    uint8_t match_number;
    std::map<uint8_t, uint8_t> winners_id_count;

    void pop_and_process_all();

    void push_responce(Snapshot& actual_status);

    void check_for_winner(Snapshot&);

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

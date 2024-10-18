#include <map>
#include "common/blocking_queue.h"
#include "common/thread.h"
#include "common/snapshot.h"
#include "common/commands.h"
#include "action.h"
#include "list_monitor.h"

struct Spawn{
    uint32_t x;
    uint32_t y;
    uint8_t it_since_picked;
};

struct Map{
    std::vector<std::vector<struct Block>> blocks;
    std::vector<struct Spawn> spawns;
};

class GameLoop: public Thread {
private:
    Queue<struct action>& actions_queue;
    QueueListMonitor& snaps_queue_list;
    struct Snapshot ducks_info;
    struct Map map_info;

    void load_map();

    void verify_spawn();
    void push_responce();
    void process_action(struct action& action);
    void pop_and_process_all();


public:
    GameLoop(Queue<struct action>& game_queue, QueueListMonitor& queue_list);
    /*   This class is the game loop.
     *   Start a loop that pop a messaje of the game queue, process the request and
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
#include <map>
#include "common/blocking_queue.h"
#include "common/thread.h"
#include "common/snapshot.h"
#include "common/commands.h"
#include "action.h"
#include "list_monitor.h"

struct Rectangle {
    uint32_t x;
    uint32_t y;
    uint32_t wide;
    uint32_t high;
};

struct Spawn{
    uint32_t x;
    uint32_t y;
    uint16_t it_since_picked;
    bool picked;
};

struct Block{
    uint8_t type; // 0 para casilla vacia
    struct Rectangle rectangle;
};

struct Map{
    std::vector<std::vector<struct Block>> blocks;
    std::vector<struct Spawn> spawns;
    uint32_t rows;
    uint32_t columns;
};

struct Colition{
    bool vertical_colition;
    bool horizontal_colition;
};

class GameLoop: public Thread {
private:
    Queue<struct action>& actions_queue;
    QueueListMonitor& snaps_queue_list;
    struct Snapshot ducks_info;
    struct Map map_info;

    void load_map();

    struct Colition check_near_blocks_colition(struct Rectangle &duck, uint32_t new_x, uint32_t new_y);

    void verify_spawn();
    void push_responce();
    void process_action(struct action& action);
    void update_game_status();
    void pop_and_process_all();

public:
    GameLoop(Queue<struct action>& game_queue, QueueListMonitor& queue_list, uint8_t players_quantity);
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
#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <cstdint>
#include <vector>

#include "../action.h"
#include "common/map_dto.h"
#include "common/snapshot.h"

#include "collectables_manager.h"
#include "collisions.h"
#include "duck_player.h"

struct Spawn {
    int16_t x;
    int16_t y;
    uint16_t it_since_picked;
    uint16_t it_to_spawn;
    bool picked;
    uint32_t collectable_id;
};

class GameOperator {
private:
    std::vector<DuckPlayer> players;
    CollisionChecks collisions;
    CollectablesManager collectables;
    std::vector<Spawn> spawns;

    void verify_spawn();
    void check_spawn_picked(uint32_t id);
    GunType get_random_guntype();

    void load_map(Map& map_dto);
    void initialize_players(uint8_t players_quantity);
public:
    GameOperator();
    void initialize_game(Map& map_dto, uint8_t players_quantity);
    void process_action(action& action);
    void update_game_status();

    void get_snapshot(Snapshot& snapshot);

    ~GameOperator();
};


#endif

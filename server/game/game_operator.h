#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "common/commands.h"
#include "common/map.h"
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
    std::unordered_map<uint8_t, DuckPlayer> players;
    CollisionChecks collisions;
    CollectablesManager collectables;
    std::vector<Spawn> spawns;

    void verify_spawn();
    void check_spawn_picked(uint32_t id);
    GunType get_random_guntype();

    void load_map(Map& map_dto);
    void initialize_players(const std::vector<std::pair<uint8_t, std::string>>& ducks_info, Map& map_info);

public:
    GameOperator();
    void initialize_game(Map& map_info, const std::vector<std::pair<uint8_t, std::string>>& ducks_info);
    void process_action(action& action);
    void update_game_status();

    void get_snapshot(Snapshot& snapshot);
    void delete_duck_player(uint8_t id_duck);
    ~GameOperator();
};


#endif

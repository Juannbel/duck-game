#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "common/commands.h"
#include "common/map.h"
#include "common/snapshot.h"
#include "server/game/boxes.h"

#include "collectables_manager.h"
#include "collisions.h"
#include "duck_player.h"

class Spawn {
public:
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
    std::unordered_map<uint32_t, BoxEntity> boxes;

    void verify_spawn();
    void check_broken_boxes();
    void check_spawn_picked(uint32_t id);
    GunType get_random_guntype(bool with_exploded_grenade);

    void load_map(const Map& map_dto);     
    void initialize_players(const std::vector<std::pair<uint8_t, std::string>>& ducks_info,
                            const Map& map_info, bool first_round);
    void initialize_boxes(const Map& map_info);
    void handle_cheat(DuckPlayer& duck, Command command);

public:
    GameOperator();
    void initialize_game(const Map& map_info,
                         const std::vector<std::pair<uint8_t, std::string>>& ducks_info, bool first_round);
    void check_start_game(Snapshot& actual_status);
    void process_action(action& action);
    void update_game_status();

    void get_snapshot(Snapshot& snapshot);
    void delete_duck_player(uint8_t id_duck);
    ~GameOperator();
};


#endif

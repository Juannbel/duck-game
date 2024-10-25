#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <cstdint>
#include <map>
#include <vector>
#include "common/map_dto.h"
#include "common/snapshot.h"
#include "map_collisions.h"
#include "server/duck_player.h"
#include "server/action.h"

struct GunPosition {
    int16_t x;
    int16_t y;
    uint32_t id;
};

struct Spawn {
    int16_t x;
    int16_t y;
    uint16_t it_since_picked;
    bool picked;
};

class EntityManager {
private:
    std::vector<DuckPlayer> players;
    MapCollisions map_collisions;
    std::map<uint32_t, Bullet> bullets;
    std::vector<Spawn> spanws;

    void verify_spawn();
public:
    EntityManager(Map& map_dto, uint8_t players_quantity);
    void process_action(action &action);
    void update_game_status();

    void get_snapshot(Snapshot &snapshot);

    ~EntityManager();
};


#endif

#ifndef BULLET_MANAGER_H
#define BULLET_MANAGER_H

#include <cstdint>
#include <map>
#include <unordered_map>

#include <sys/types.h>

#include "common/snapshot.h"
#include "server/game/bullet_entity.h"
#include "server/game/boxes.h"

#include "collisions.h"
class DuckPlayer;

class BulletManager {
private:
    uint32_t bullet_id;
    std::map<uint32_t, BulletEntity> bullets;
    CollisionChecks& collisions;
    std::unordered_map<uint8_t, DuckPlayer>& ducks;
    std::unordered_map<uint32_t, BoxEntity>& boxes;

public:
    explicit BulletManager(CollisionChecks&, std::unordered_map<uint8_t, DuckPlayer>& ducks, std::unordered_map<uint32_t, BoxEntity>& boxes);
    void clear_bullets();
    void add_bullet(const Rectangle& hb, int16_t angle, GunType type, uint16_t range, uint8_t damage);
    void update_bullets();
    void add_bullets_to_snapshot(Snapshot&);
};

#endif

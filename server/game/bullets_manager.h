#ifndef BULLET_MANAGER_H
#define BULLET_MANAGER_H

#include <cstdint>
#include <map>
#include <unordered_map>

#include <sys/types.h>

#include "common/snapshot.h"

#include "collisions.h"
class DuckPlayer;

struct BulletInfo {
    Bullet status;
    Rectangle hitbox;
    // cppcheck-suppress unusedStructMember
    float speed;
    // cppcheck-suppress unusedStructMember
    uint8_t damage;
    // cppcheck-suppress unusedStructMember
    bool is_alive;
};


class BulletManager {
private:
    uint32_t bullet_id;
    std::map<uint32_t, BulletInfo> bullets;
    CollisionChecks& collisions;
    std::unordered_map<uint8_t, DuckPlayer>& ducks;

    bool check_collision_with_ducks(Rectangle& bullet, uint8_t damage);

public:
    explicit BulletManager(CollisionChecks&, std::unordered_map<uint8_t, DuckPlayer>& ducks);
    void clear_bullets();
    void add_bullet(BulletInfo&);
    void update_bullets();
    void add_bullets_to_snapshot(Snapshot&);
};

#endif

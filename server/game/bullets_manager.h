#ifndef BULLET_MANAGER_H
#define BULLET_MANAGER_H

#include <map>
#include <set>
#include <cstdint>
#include "common/snapshot.h"
#include "collisions.h"

struct BulletInfo {
    Bullet status;
    Rectangle hitbox;
    uint8_t speed;
};


class BulletManager {
private:
    uint32_t bullet_id;
    std::map<uint32_t, BulletInfo> bullets;
    CollisionChecks& collisions;

public:
    BulletManager(CollisionChecks&);

    void add_bullet(BulletInfo&);
    void update_bullets();
    void add_bullets_to_snapshot(Snapshot&);
};

#endif

#ifndef COLLECTABLES_MANAGER_H
#define COLLECTABLES_MANAGER_H

#include "common/snapshot.h"
#include "collisions.h"
#include "gun_entity.h"

#include <map>
#include <vector>
#include <cstdint>

class CollectablesManager{
private:
    std::map<uint32_t, GunEntity> guns;
    std::map<uint32_t, Bullet> bullets;
    CollisionChecks& collisions;
    uint32_t collectable_id;
    uint32_t bullet_id;

    GunEntity new_gun(Gun& gun);

public:
    explicit CollectablesManager(CollisionChecks& collision);
    
    uint32_t get_and_inc_collectable_id();
    void add_gun(Gun& gun);
    void add_gun(GunEntity&& gun);
    void drop_gun(GunEntity &&gun, const Rectangle& duck_hitbox);
    void move_guns_falling();
    GunEntity pickup(const Rectangle &duck);
    void add_guns_to_snapshot(Snapshot& snapshot);
};

#endif

#ifndef COLLECTABLES_MANAGER_H
#define COLLECTABLES_MANAGER_H

#include <cstdint>
#include <map>
#include <memory>
#include <set>
#include <unordered_map>

#include "common/snapshot.h"

#include "bullets_manager.h"
#include "collisions.h"
#include "gun_entity.h"

class CollectablesManager {
private:
    std::map<uint32_t, std::shared_ptr<GunEntity>> guns;
    std::set<uint32_t> picked_up_guns;
    CollisionChecks& collisions;
    BulletManager bullets;
    uint32_t collectable_id;

    void new_gun(Gun& gun);

public:
    explicit CollectablesManager(CollisionChecks& collision,
                                 std::unordered_map<uint8_t, DuckPlayer>& ducks);
    void reset_collectables();
    uint32_t get_and_inc_collectable_id();
    void add_gun(Gun& gun);
    void drop_gun(std::shared_ptr<GunEntity> gun, const Rectangle& duck_hitbox);
    void move_guns_falling();
    std::shared_ptr<GunEntity> pickup(const Rectangle& duck);
    void add_guns_to_snapshot(Snapshot& snapshot);
};

#endif

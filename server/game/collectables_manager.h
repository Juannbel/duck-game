#ifndef COLLECTABLES_MANAGER_H
#define COLLECTABLES_MANAGER_H

#include <cstdint>
#include <map>
#include <memory>
#include <set>
#include <unordered_map>

#include <SDL2/SDL_stdinc.h>

#include "common/snapshot.h"

#include "boxes.h"
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
    CollectablesManager(CollisionChecks& collision, std::unordered_map<uint8_t, DuckPlayer>& ducks,
                        std::unordered_map<uint32_t, BoxEntity>& boxes);
    void reset_collectables();
    uint32_t add_gun(Gun& gun);
    void drop_gun(std::shared_ptr<GunEntity> gun, const Rectangle& duck_hitbox);
    void update_guns_and_bullets();
    std::shared_ptr<GunEntity> pickup(const Rectangle& duck);
    void add_guns_to_snapshot(Snapshot& snapshot);
};

#endif

#include "bullets_manager.h"

#include <cmath>
#include <cstdint>
#include <vector>

#include "common/snapshot.h"
#include "server/game/bullet_entity.h"
#include "server/game/collisions.h"

#include "duck_player.h"


BulletManager::BulletManager(CollisionChecks& collisions,
                             std::unordered_map<uint8_t, DuckPlayer>& ducks,
                             std::unordered_map<uint32_t, BoxEntity>& boxes):
        bullet_id(), collisions(collisions), ducks(ducks), boxes(boxes) {}

void BulletManager::clear_bullets() {
    bullet_id = 0;
    bullets.clear();
}

void BulletManager::add_bullet(const Rectangle& info, int16_t angle, GunType type) {
    ++bullet_id;
    bullets.emplace(bullet_id,
                    BulletEntity(info, collisions, ducks, boxes, angle, type, bullet_id));
}

void BulletManager::update_bullets() {
    std::vector<uint32_t> id_to_eliminate;
    for (auto& [id, bullet]: bullets) {
        bullet.update_status();
        if (!bullet.still_alive()) {
            id_to_eliminate.push_back(id);
        }
    }
    for (uint32_t id: id_to_eliminate) {
        bullets.erase(id);
    }
}

void BulletManager::add_bullets_to_snapshot(Snapshot& snaphot) {
    for (auto& [id, bullet]: bullets) {
        snaphot.bullets.push_back(bullet.get_status());
    }
}

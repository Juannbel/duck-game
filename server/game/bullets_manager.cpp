#include "bullets_manager.h"
#include "duck_player.h"


#include <cmath>
#include <cstdint>
#include <vector>
#include "common/snapshot.h"
#include "server/game/collisions.h"

const uint8_t bullet_updates_per_it = 3;

BulletManager::BulletManager(CollisionChecks& collisions, std::vector<DuckPlayer>& ducks): bullet_id(), collisions(collisions), ducks(ducks) {}

void BulletManager::add_bullet(BulletInfo& bullet) {
    ++bullet_id;
    bullet.status.bullet_id = bullet_id;
    bullets[bullet_id] = bullet;
}

void BulletManager::check_collision_with_ducks(Rectangle& bullet, uint8_t damage) {
    for(auto &duck : ducks) {
        duck.get_hit(bullet, damage);
    }
}

void BulletManager::update_bullets() {
    std::vector<uint32_t> id_to_eliminate;
    for (auto& [id, bullet]: bullets) {
        Rectangle& hitbox = bullet.hitbox;
        float angle = static_cast<float>(bullet.status.angle);
        for (int i = 0; i < bullet_updates_per_it; i++) {
            float new_x = hitbox.coords.x + (bullet.speed / bullet_updates_per_it) *
                                                    std::cos(angle * (std::numbers::pi / 180));
            float new_y = hitbox.coords.y - (bullet.speed / bullet_updates_per_it) *
                                                    std::sin(angle * (std::numbers::pi / 180));
            Collision collision = collisions.check_near_blocks_collision(hitbox, new_x, new_y);
            if (collision.vertical_collision || collision.horizontal_collision ||
                collisions.out_of_map(new_x, new_y)) {
                id_to_eliminate.push_back(id);
                break;
            }
            hitbox.coords.x = new_x;
            hitbox.coords.y = new_y;
            check_collision_with_ducks(hitbox, bullet.damage);
        }
        bullet.status.x = static_cast<int16_t>(hitbox.coords.x);
        bullet.status.y = static_cast<int16_t>(hitbox.coords.y);
    }
    for (uint32_t id: id_to_eliminate) {
        bullets.erase(id);
    }
}

void BulletManager::add_bullets_to_snapshot(Snapshot& snaphot) {
    for (auto& [id, bullet]: bullets) {
        snaphot.bullets.push_back(bullet.status);
    }
}

#include "bullets_manager.h"

#include <cmath>

BulletManager::BulletManager(CollisionChecks& collisions) : bullet_id(), collisions(collisions) {}

BulletManager& BulletManager::operator=(BulletManager&& other) {
    bullet_id = bullet_id;

    return *this;
}

void BulletManager::add_bullet(BulletInfo& bullet) {
    ++bullet_id;
    bullets[bullet_id] = bullet;
}

void BulletManager::update_bullets() {
    std::vector<uint32_t> id_to_eliminate;
    for (auto &[id, bullet] : bullets) {
        Rectangle &hitbox = bullet.hitbox;
        float angle = static_cast<float>(bullet.status.angle);
        for (int i = 0; i < bullet.speed; i++) {
            float new_x = hitbox.coords.x + std::cos(angle * (std::numbers::pi/180));
            float new_y = hitbox.coords.y + std::sin(angle * (std::numbers::pi/180));
            Collision collision = collisions.check_near_blocks_collision(hitbox, new_x, new_y);
            if (collision.vertical_collision || collision.horizontal_collision || collisions.out_of_map(new_x, new_y)) {
                id_to_eliminate.push_back(id);
                break;
            }
            hitbox.coords.x = new_x;
            hitbox.coords.y = new_y;
        }
        bullet.status.x = static_cast<int16_t>(hitbox.coords.x);
        bullet.status.x = static_cast<int16_t>(hitbox.coords.x);
    }
    for (uint32_t id : id_to_eliminate) {
        bullets.erase(id);
    }
}
void BulletManager::add_bullets_to_snapshot(Snapshot& snaphot) {
    for (auto &[id, bullet] : bullets) {
        snaphot.bullets.push_back(bullet.status);
    }
    
}


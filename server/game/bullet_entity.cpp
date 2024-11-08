#include "bullet_entity.h"
#include <cstdint>
#include <sys/types.h>
#include <cmath>
#include "common/snapshot.h"
#include "server/game/collisions.h"
#include "server/game/duck_player.h"

const int SPEED_PER_IT = 3;
const uint8_t bullet_updates_per_it = 3;


BulletEntity::BulletEntity(const Duck& info, CollisionChecks& collision_ckecker, std::unordered_map<uint8_t, DuckPlayer>& ducks, int16_t angle, GunType type, uint32_t id) : 
    status(), hitbox(), speed(9), damage(50), is_alive(true), collisions(collision_ckecker), ducks(ducks) {
    int16_t x = info.facing_right ? info.x + DUCK_HITBOX_WIDTH + 1 :
                                      info.x - BULLET_HITBOX_WIDTH - 1;
    int16_t y =  info.facing_up ? info.y - BULLET_HITBOX_HEIGHT-1 : info.y + DUCK_LAYED_HITBOX_HEIGHT;
    status.bullet_id = id;
    hitbox.coords.x = x;
    hitbox.coords.y = y;
    hitbox.height = BULLET_HITBOX_HEIGHT;
    hitbox.width = BULLET_HITBOX_WIDTH;
    status = {0, x, y, static_cast<uint16_t>(angle % 360), type};
}

bool BulletEntity::check_collision_with_ducks() {
    for (auto& [id, duck] : ducks) {
        if (duck.get_hit(hitbox, damage)) {
            return true;
        }
    }
    return false;
}

void BulletEntity::update_status() {
    float angle = static_cast<float>(status.angle);
    for (int i = 0; i < speed/SPEED_PER_IT; i++) {
        float new_x = hitbox.coords.x + (SPEED_PER_IT) *
                                                std::cos(angle * (std::numbers::pi / 180));
        float new_y = hitbox.coords.y - (SPEED_PER_IT) *
                                                std::sin(angle * (std::numbers::pi / 180));
        Collision collision = collisions.check_near_blocks_collision(hitbox, new_x, new_y);
        if (collision.vertical_collision || collision.horizontal_collision ||
            collisions.out_of_map(new_x, new_y)) {
            is_alive = false;
        }
        hitbox.coords.x = new_x;
        hitbox.coords.y = new_y;
        bool hit = check_collision_with_ducks();
        if (hit || !is_alive) {
            is_alive = false;
            break;
        }
    }
    status.x = static_cast<int16_t>(hitbox.coords.x);
    status.y = static_cast<int16_t>(hitbox.coords.y);
}

Bullet& BulletEntity::get_status() {
    return status;
}

bool BulletEntity::still_alive() {
    return is_alive;
}

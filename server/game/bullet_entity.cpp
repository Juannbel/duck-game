#include "bullet_entity.h"

#include <cmath>
#include <cstdint>

#include <sys/types.h>

#include "common/snapshot.h"
#include "server/game/collisions.h"
#include "server/game/duck_player.h"

#include "ticks.h"

#define SPEED_PER_TICK(x) (x / TICKS)

const int PARTIAL_MOVE = 4;
const int SPEED_PER_IT = TICKS / 30;
const float GUN_FALL_SPEED = 120.0f / TICKS;
const float GUN_THROW_SPEED = 120.0f / TICKS;

BulletEntity::BulletEntity(const Duck& info, CollisionChecks& collision_ckecker,
                           std::unordered_map<uint8_t, DuckPlayer>& ducks, int16_t angle,
                           GunType type, uint32_t id, uint16_t range):
        status(),
        hitbox(),
        speed(SPEED_PER_TICK(360.0f)),
        damage(50),
        range(range),
        is_alive(true),
        collisions(collision_ckecker),
        ducks(ducks) {
    int16_t x =
            info.facing_right ? info.x + DUCK_HITBOX_WIDTH + 1 : info.x - BULLET_HITBOX_WIDTH - 1;
    int16_t y =
            info.facing_up ? info.y - BULLET_HITBOX_HEIGHT - 1 : info.y + DUCK_LAYED_HITBOX_HEIGHT;
    hitbox.coords.x = x;
    hitbox.coords.y = y;
    hitbox.height = BULLET_HITBOX_HEIGHT;
    hitbox.width = BULLET_HITBOX_WIDTH;
    status = {id, x, y, static_cast<uint16_t>(angle % 360), type};
}

BulletEntity::BulletEntity(const Rectangle& info, CollisionChecks& collision_ckecker,
                           std::unordered_map<uint8_t, DuckPlayer>& ducks, int16_t angle,
                           GunType type, uint32_t id, uint16_t range):
        status(),
        hitbox(info),
        speed(SPEED_PER_TICK(480.0f)),
        damage(50),
        range(range),
        is_alive(true),
        collisions(collision_ckecker),
        ducks(ducks) {
    status = {id, static_cast<int16_t>(info.coords.x), static_cast<int16_t>(info.coords.y),
              static_cast<uint16_t>(angle % 360), type};
}

bool BulletEntity::check_collision_with_ducks() {
    for (auto& [id, duck]: ducks) {
        if (duck.get_hit(hitbox, damage)) {
            if (status.type == Banana) {
                duck.slide();
            }
            return true;
        }
    }
    return false;
}

void BulletEntity::update_angle(float new_x, Collision collision) {
    Coordenades& coords = collision.last_valid_position;
    if (collision.horizontal_collision || collision.vertical_collision) {
        uint16_t diff = status.angle % 90;
        if (diff == 0) {
            status.angle = (status.angle + 180) % 360;
        } else if (new_x == coords.x && !collision.vertical_collision) {
            if (status.angle < 90 || (status.angle > 180 && status.angle < 270)) {
                status.angle = (status.angle + 180 - 2 * diff) % 360;
            } else {
                status.angle -= 2 * diff;
            }
        } else {
            if (status.angle < 90 || (status.angle > 180 && status.angle < 270)) {
                status.angle = (status.angle + 180 + 2 * diff) % 360;
            } else {
                status.angle = (status.angle + 180 - 2 * diff) % 360;
            }
        }
        hitbox.coords.x = coords.x;
        hitbox.coords.y = coords.y;
    }
}

void BulletEntity::check_collision_and_change_angle(float new_x, float new_y) {
    Collision collision = collisions.check_near_blocks_collision(hitbox, new_x, new_y);
    hitbox.coords.x = new_x;
    hitbox.coords.y = new_y;
    if (status.type == PewPewLaser || status.type == LaserRifle) {
        update_angle(new_x, collision);
    } else if (collision.vertical_collision || collision.horizontal_collision) {
        is_alive = false;
    }
}

void BulletEntity::move_banana() {
    float new_x = hitbox.coords.x;
    if (range) {
        float move_x =
                GUN_THROW_SPEED * (static_cast<float>(range) / (static_cast<float>(TICKS) / 2));
        new_x = status.angle == 0 ? new_x + move_x : new_x - move_x;
        --range;
    }
    float new_y = hitbox.coords.y + GUN_FALL_SPEED;
    hitbox.coords =
            collisions.check_near_blocks_collision(hitbox, new_x, new_y).last_valid_position;
    hitbox.coords = collisions.check_near_blocks_collision(hitbox, new_x, hitbox.coords.y)
                            .last_valid_position;
}

void BulletEntity::update_status() {
    if (status.type == Banana) {
        move_banana();
        if (check_collision_with_ducks()) {
            is_alive = false;
        }
        status.x = static_cast<int16_t>(hitbox.coords.x);
        status.y = static_cast<int16_t>(hitbox.coords.y);
        return;
    }
    for (int i = 0; i < speed / PARTIAL_MOVE; i++) {
        float angle = static_cast<float>(status.angle);
        float new_x = hitbox.coords.x + (PARTIAL_MOVE)*std::cos(angle * (std::numbers::pi / 180));
        float new_y = hitbox.coords.y - (PARTIAL_MOVE)*std::sin(angle * (std::numbers::pi / 180));
        check_collision_and_change_angle(new_x, new_y);
        if (collisions.out_of_map(new_x, new_y)) {
            is_alive = false;
        }
        range = range > PARTIAL_MOVE ? range - PARTIAL_MOVE : 0;
        bool hit = check_collision_with_ducks();
        if (hit || !is_alive || range == 0) {
            is_alive = false;
            break;
        }
    }
    status.x = static_cast<int16_t>(hitbox.coords.x);
    status.y = static_cast<int16_t>(hitbox.coords.y);
}

Bullet& BulletEntity::get_status() { return status; }

bool BulletEntity::still_alive() { return is_alive; }

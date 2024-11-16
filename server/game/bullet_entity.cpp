#include "bullet_entity.h"

#include <cmath>
#include <cstdint>

#include <sys/types.h>

#include "common/config.h"
#include "common/snapshot.h"
#include "server/game/boxes.h"
#include "server/game/collisions.h"
#include "server/game/duck_player.h"

static Config& config = Config::get_instance();

const static int TICKS = config.get_server_ticks();
const int PARTIAL_MOVE = 4;
const static float GUN_FALL_SPEED = (config.get_fall_speed() * BLOCK_SIZE) / TICKS;
const static float GUN_THROW_SPEED = (config.get_fall_speed() * BLOCK_SIZE) / TICKS;

#define SPEED_PER_TICK(x) (x / TICKS)

BulletEntity::BulletEntity(const Rectangle& info, CollisionChecks& collision_ckecker,
                           std::unordered_map<uint8_t, DuckPlayer>& ducks,
                           std::unordered_map<uint32_t, BoxEntity>& boxes, int16_t angle,
                           GunType type, uint32_t id):
        status({id, static_cast<int16_t>(info.coords.x), static_cast<int16_t>(info.coords.y),
                static_cast<uint16_t>(angle % 360), type}),
        hitbox(info),
        speed(SPEED_PER_TICK(config.get_bullet_speed(type) * BLOCK_SIZE)),
        damage(config.get_bullet_damage(type)),
        range(config.get_bullet_range(type) * BLOCK_SIZE),
        is_alive(true),
        collisions(collision_ckecker),
        ducks(ducks),
        boxes(boxes) {}

bool BulletEntity::check_collision_with_boxes_ducks() {
    for (auto& [id, duck]: ducks) {
        if (duck.get_hit(hitbox, damage)) {
            if (status.type == Banana) {
                duck.slide();
            }
            return true;
        }
    }
    for (auto& [id, box]: boxes) {
        if (box.get_hit(hitbox, damage)) {
            return true;
        }
    }
    return false;
}

void BulletEntity::update_angle(Collision collision) {
    const Coordenades& coords = collision.last_valid_position;
    if (collision.horizontal_collision || collision.vertical_collision) {
        uint16_t diff = status.angle % 90;
        if (diff == 0) {
            status.angle = (status.angle + 180) % 360;
        } else if (!collision.vertical_collision) {
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
    if (status.type == PewPewLaser || status.type == LaserRifle || status.type == DeathLaser) {
        update_angle(collision);
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
        if (check_collision_with_boxes_ducks()) {
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
        bool hit = check_collision_with_boxes_ducks();
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

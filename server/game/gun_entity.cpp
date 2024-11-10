#include "gun_entity.h"

#include <cstdint>
#include <cstdlib>
#include <random>

#include "common/shared_constants.h"
#include "common/snapshot.h"
#include "server/game/collisions.h"
#include "ticks.h"

const float GUN_FALL_SPEED = 120.0f / TICKS;
const float GUN_THROW_SPEED = 240.0f / TICKS;


#include "duck_player.h"

GunEntity::GunEntity(Gun& gun, BulletManager* bullets, CollisionChecks& collisions):
        id(gun.gun_id),
        type(gun.type),
        hitbox(),
        facing_right(),
        it_mooving(),
        ammo(),
        range(),
        bullets_to_shoot(),
        shooted_bullets(),
        initial_angle(),
        inaccuracy(),
        trigger_pulled(),
        ready_to_shoot(),
        it_since_shoot(),
        it_to_shoot(),
        it_to_reload(),
        it_reloading(),
        bullets(bullets),
        collisions(collisions) 
        {
            hitbox.coords.x = gun.x;
            hitbox.coords.y = gun.y;
            hitbox.height = COLLECTABLE_HITBOX_HEIGHT;
            hitbox.width = COLLECTABLE_HITBOX_WIDTH;
        }

int16_t GunEntity::get_rand_angle() {
    if (inaccuracy == 0)
        return 0;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-inaccuracy, inaccuracy);  // 0 es None
    return static_cast<int16_t>(dis(gen));
}

void GunEntity::add_bullet(DuckPlayer& player) {
    if ((trigger_pulled && it_since_shoot > it_to_shoot) ||
        (bullets_to_shoot > shooted_bullets && shooted_bullets > 0)) {
        const Duck& status = player.get_status();
        int16_t angle = status.facing_right ? 0 : 180;
        angle = status.facing_up ? 90 : angle;
        angle += get_rand_angle();
        bullets->add_bullet(status, angle, type, range);
        it_since_shoot = 0;
        it_reloading = 0;
        ++shooted_bullets;
        if (ammo > 0) {
            --ammo;
        }
    }
    if (bullets_to_shoot == shooted_bullets) {
        shooted_bullets = 0;
    }
    if (it_to_shoot > 0) {
        ++it_since_shoot;
    }
    if (it_reloading < it_to_reload) {
        ++it_reloading;
    }
}

void GunEntity::check_movement() {
    float new_x = hitbox.coords.x;
    if (it_mooving) {
        float move_x = GUN_THROW_SPEED * (static_cast<float>(it_mooving) / (static_cast<float>(TICKS)/2));
        new_x = facing_right ? new_x + move_x : new_x - move_x;
        --it_mooving;
    }
    float new_y = hitbox.coords.y + GUN_FALL_SPEED;
    hitbox.coords = collisions.check_near_blocks_collision(hitbox, new_x, new_y).last_valid_position;
    hitbox.coords = collisions.check_near_blocks_collision(hitbox, new_x, hitbox.coords.y).last_valid_position;
}

void GunEntity::explode_grenade() {
    Rectangle hb = hitbox;
    hb.height = BULLET_HITBOX_HEIGHT;
    hb.width = BULLET_HITBOX_WIDTH;
    for (int i = 0; i < 30; ++i) {
        int16_t angle = 0;
        angle += get_rand_angle();
        bullets->add_bullet(hb, angle, type, range);
    }
    destroy();
}

void GunEntity::update_status() {
    check_movement();
    if (type == Grenade) {
        if (it_since_shoot == it_to_shoot) {
            explode_grenade();
        } else if(trigger_pulled) {
            ++it_since_shoot;
        }
    }
}

void GunEntity::trhow(bool facing_right) {
    this->facing_right = facing_right;
    it_mooving = TICKS / 2;
    if (it_since_shoot > 0 && type == Banana) {
        Rectangle b_hitbox = hitbox;
        b_hitbox.height = BULLET_HITBOX_HEIGHT;
        b_hitbox.width = BULLET_HITBOX_WIDTH;
        b_hitbox.coords.x = facing_right ? b_hitbox.coords.x + DUCK_HITBOX_WIDTH + 2 : b_hitbox.coords.x - BULLET_HITBOX_WIDTH - 2;
        int16_t angle = facing_right ? 0 : 180;
        bullets->add_bullet(b_hitbox, angle, type, range);
        destroy();
    }
}

void GunEntity::destroy() {
    type = None;
    hitbox.coords.x = 0;
    hitbox.coords.y = 0;
    ammo = 0;
    trigger_pulled = false;
}

void GunEntity::set_new_coords(float x, float y) {
    hitbox.coords.x = x;
    hitbox.coords.y = y;
    if (type == Banana && it_since_shoot > 0) {
        hitbox.coords.x = facing_right ? hitbox.coords.x + DUCK_HITBOX_WIDTH + 1 : hitbox.coords.x - BULLET_HITBOX_WIDTH - 1;
    }
}

Gun GunEntity::get_gun_info() {
    Gun gun_info = {id, type, static_cast<int16_t>(hitbox.coords.x), static_cast<int16_t>(hitbox.coords.y)};
    return gun_info;
}

const Rectangle& GunEntity::get_hitbox() {
    return hitbox;
}

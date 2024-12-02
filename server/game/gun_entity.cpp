#include "gun_entity.h"

#include <cstdint>
#include <cstdlib>
#include <random>

#include "common/config.h"
#include "common/shared_constants.h"
#include "common/snapshot.h"
#include "server/game/collisions.h"

static Config& config = Config::get_instance();

const static int TICKS = config.get_server_ticks();
const float GUN_FALL_SPEED = (config.get_fall_speed() * BLOCK_SIZE) / TICKS;
const float GUN_THROW_SPEED = (config.get_fall_speed() * BLOCK_SIZE) * 2 / TICKS;


#include "duck_player.h"

GunEntity::GunEntity(Gun& gun, BulletManager* bullets, CollisionChecks& collisions):
        infinite_ammo_activated(),
        id(gun.gun_id),
        type(gun.type),
        hitbox(),
        facing_right(),
        stuck(),
        it_mooving(),
        ammo(config.get_gun_ammo(type)),
        knockback(config.get_gun_knockback(type) * BLOCK_SIZE),
        bullets_to_shoot(),
        shooted_bullets(),
        initial_angle(),
        inaccuracy(config.get_gun_recoil(type)),
        trigger_pulled(),
        ready_to_shoot(),
        it_since_shoot(),
        it_to_shoot(),
        it_reloading(),
        bullets(bullets),
        collisions(collisions) {
    hitbox.coords.x = gun.x;
    hitbox.coords.y = gun.y;
    hitbox.height = COLLECTABLE_HITBOX_HEIGHT;
    hitbox.width = COLLECTABLE_HITBOX_WIDTH;
}

void GunEntity::infinite_ammo() { infinite_ammo_activated = !infinite_ammo_activated; }

void GunEntity::start_shooting() {
    it_since_shoot = trigger_pulled ? it_since_shoot : 0;
    shooted_bullets = trigger_pulled ? shooted_bullets : bullets_to_shoot;
    trigger_pulled = true;
}

void GunEntity::stop_shooting() { trigger_pulled = false; }

int16_t GunEntity::get_rand_angle() {
    if (inaccuracy == 0)
        return 0;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-inaccuracy, inaccuracy);
    return static_cast<int16_t>(dis(gen));
}

int16_t GunEntity::calculate_initial_angle(bool facing_right, bool facing_up) {
    int16_t angle = facing_right ? 0 : 180;
    angle = !facing_up ? angle : facing_right ? 70 : 110;
    return angle;
}

void GunEntity::add_bullet(const Rectangle& player_hb, int16_t angle, bool facing_right,
                           bool facing_up) {
    if (!it_since_shoot && shooted_bullets && ammo) {
        Rectangle b_hb{};
        b_hb.coords.x = facing_right ? player_hb.coords.x + DUCK_HITBOX_WIDTH + 1 :
                                       player_hb.coords.x - BULLET_HITBOX_WIDTH - 1;
        b_hb.coords.y = facing_up ? player_hb.coords.y - BULLET_HITBOX_HEIGHT - 1 :
                                    player_hb.coords.y + DUCK_LAYED_HITBOX_HEIGHT - 2;
        b_hb.height = BULLET_HITBOX_HEIGHT;
        b_hb.width = BULLET_HITBOX_WIDTH;
        it_since_shoot = it_to_shoot;
        --shooted_bullets;
        if (!infinite_ammo_activated && type != DeathLaser) {
            --ammo;
        }
        if (type == LaserRifle || type == PewPewLaser || type == DeathLaser) {
            Collision verify_coords =
                    collisions.check_near_blocks_collision(b_hb, b_hb.coords.x, b_hb.coords.y);
            if (verify_coords.horizontal_collision && verify_coords.vertical_collision) {
                return;
            }
        }
        bullets->add_bullet(b_hb, angle, type);
    }
    if (it_to_shoot && it_since_shoot) {
        --it_since_shoot;
    }
}

void GunEntity::check_movement() {
    if (stuck) {
        return;
    }
    float new_x = hitbox.coords.x;
    if (it_mooving) {
        float move_x = GUN_THROW_SPEED *
                       (static_cast<float>(it_mooving) / (static_cast<float>(TICKS) / 2));
        new_x = facing_right ? new_x + move_x : new_x - move_x;
        --it_mooving;
    }
    float new_y = hitbox.coords.y + GUN_FALL_SPEED;
    hitbox.coords = collisions.check_near_blocks_collision(hitbox, hitbox.coords.x, new_y)
                            .last_valid_position;
    hitbox.coords = collisions.check_near_blocks_collision(hitbox, new_x, hitbox.coords.y)
                            .last_valid_position;

    if (collisions.out_of_map(hitbox.coords.x, hitbox.coords.y)) {
        destroy();
    }
}

void GunEntity::update_status() { check_movement(); }

void GunEntity::throw_gun(bool facing_right) {
    this->facing_right = facing_right;
    it_mooving = TICKS / 2;
}

void GunEntity::destroy() {
    type = None;
    hitbox.coords.x = 0;
    hitbox.coords.y = 0;
    ammo = 0;
    trigger_pulled = false;
}

void GunEntity::drop_gun(float x, float y) {
    infinite_ammo_activated = false;
    shooted_bullets = 0;
    stop_shooting();
    stuck = false;
    float new_x = x;
    new_x -= (static_cast<float>(DUCK_HITBOX_WIDTH) / 2);
    y -= (hitbox.height);
    hitbox.coords.y = y;
    hitbox.coords.x = new_x;
    Collision coll = collisions.check_near_blocks_collision(hitbox, new_x, y);
    if (coll.vertical_collision && coll.horizontal_collision) {
        new_x = x + DUCK_HITBOX_WIDTH - hitbox.width - 1;
        coll = collisions.check_near_blocks_collision(hitbox, new_x, y);
        if (coll.vertical_collision && coll.horizontal_collision) {
            new_x = x + 1;
            coll = collisions.check_near_blocks_collision(hitbox, new_x, y);
            stuck = coll.vertical_collision && coll.horizontal_collision;
        }
    }
    hitbox.coords.x = new_x;
}

float GunEntity::get_knockback() { return knockback; }

Gun GunEntity::get_gun_info() {
    Gun gun_info = {id, type, static_cast<int16_t>(hitbox.coords.x),
                    static_cast<int16_t>(hitbox.coords.y)};
    return gun_info;
}

const Rectangle& GunEntity::get_hitbox() { return hitbox; }

bool GunEntity::empty() { return ammo == 0; }

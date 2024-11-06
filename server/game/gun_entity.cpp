#include "gun_entity.h"

#include <cstdint>
#include <cstdlib>
#include <random>

#include "common/shared_constants.h"

#include "duck_player.h"

GunEntity::GunEntity(BulletManager* bullets):
        id(),
        type(),
        x(),
        y(),
        ammo(),
        range(),
        bullets_to_shoot(),
        shooted_bullets(),
        inaccuracy(),
        trigger_pulled(),
        ready_to_shoot(),
        it_since_shoot(),
        it_to_shoot(),
        it_to_reload(),
        it_reloading(),
        bullets(bullets) {}

GunEntity::GunEntity(Gun& gun, BulletManager* bullets):
        id(gun.gun_id),
        type(gun.type),
        x(gun.x),
        y(gun.y),
        ammo(),
        range(),
        bullets_to_shoot(),
        shooted_bullets(),
        inaccuracy(),
        trigger_pulled(),
        ready_to_shoot(),
        it_since_shoot(),
        it_to_shoot(),
        it_to_reload(),
        it_reloading(),
        bullets(bullets) {}

GunEntity::GunEntity(GunEntity&& old):
        id(old.id),
        type(old.type),
        x(old.x),
        y(old.y),
        ammo(old.ammo),
        trigger_pulled(old.trigger_pulled),
        ready_to_shoot(old.ready_to_shoot),
        it_since_shoot(old.it_since_shoot),
        bullets(old.bullets) {}

GunEntity& GunEntity::operator=(GunEntity&& old) {
    id = old.id;
    type = old.type;
    x = old.x;
    y = old.y;
    ammo = old.ammo;
    trigger_pulled = old.trigger_pulled;
    bullets = old.bullets;
    return *this;
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
        Duck status = player.get_status();
        int16_t x = status.facing_right ? status.x + DUCK_HITBOX_WIDTH + 1 :
                                          status.x - BULLET_HITBOX_WIDTH - 1;
        int16_t y = status.y + DUCK_LAYED_HITBOX_HEIGHT;
        int16_t angle = status.facing_right ? 0 : 180;
        angle = status.facing_up ? 90 : angle;

        angle += get_rand_angle();
        Rectangle hitbox;
        hitbox.coords.x = x;
        hitbox.coords.y = y;
        hitbox.height = BULLET_HITBOX_HEIGHT;
        hitbox.width = BULLET_HITBOX_WIDTH;
        Bullet bullet_status = {0, x, y, static_cast<uint16_t>(angle % 360), type};
        BulletInfo bullet = {bullet_status, hitbox, 3, 50, true};
        bullets->add_bullet(bullet);
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

void GunEntity::destroy() {
    type = None;
    x = 0;
    y = 0;
    ammo = 0;
    trigger_pulled = false;
}

void GunEntity::set_new_coords(float x, float y) {
    this->x = x;
    this->y = y;
}

Gun GunEntity::get_gun_info() {
    Gun gun_info = {id, type, static_cast<int16_t>(x), static_cast<int16_t>(y)};
    return gun_info;
}

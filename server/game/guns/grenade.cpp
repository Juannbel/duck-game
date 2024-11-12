#include "grenade.h"
#include "server/game/gun_entity.h"
#include "server/game/ticks.h"

GrenadeG::GrenadeG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions, bool explode):
        GunEntity(gun, bullets, collisions) {
    hitbox.height = GRENADE_HITBOX_HEIGHT;
    hitbox.width = GRENADE_HITBOX_WIDTH;
    ammo = 30;
    it_to_shoot = TICKS * 4;
    inaccuracy = 360;
    range = 5 * BLOCK_SIZE;
    if (explode) {
        explode_grenade();
    }
}

void GrenadeG::throw_gun(bool facing_right) {
    GunEntity::throw_gun(facing_right);
    it_mooving*=2;
}

void GrenadeG::start_shooting() {
    GunEntity::start_shooting();
    type = ActiveGrenade;
}

void GrenadeG::stop_shooting() {}

bool GrenadeG::update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) {
    (void) facing_right;
    (void) facing_up;
    hitbox.coords = player_hb.coords;
    if (it_since_shoot == it_to_shoot) {
        explode_grenade();
    }
    if (trigger_pulled && it_since_shoot < it_to_shoot) {
        ++it_since_shoot;
    }
    return false;
}

void GrenadeG::update_status() {
    check_movement();
    if (type == ActiveGrenade) {
        if (it_since_shoot == it_to_shoot) {
            explode_grenade();
        } else if (trigger_pulled) {
            ++it_since_shoot;
        }
    }
}

void GrenadeG::explode_grenade() {
    Rectangle hb = hitbox;
    hb.height = BULLET_HITBOX_HEIGHT;
    hb.width = BULLET_HITBOX_WIDTH;
    type = Grenade;
    while (ammo > 0) {
        int16_t angle = 0;
        angle += get_rand_angle();
        bullets->add_bullet(hb, angle, type, range);
        --ammo;
    }
    destroy();
}

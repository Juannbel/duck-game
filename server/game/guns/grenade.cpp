#include "grenade.h"

#include "common/snapshot.h"
#include "server/game/gun_entity.h"
#include "common/config.h"

static Config& config = Config::get_instance();

GrenadeG::GrenadeG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions, bool explode):
        GunEntity(gun, bullets, collisions) {
    hitbox.height = GRENADE_HITBOX_HEIGHT;
    hitbox.width = GRENADE_HITBOX_WIDTH;
    ammo = 30;
    it_to_shoot = config.get_server_ticks() * 4;
    inaccuracy = 360;
    if (explode) {
        it_since_shoot = it_to_shoot;
        trigger_pulled = true;
        type = ActiveGrenade;
    }
}

void GrenadeG::throw_gun(bool facing_right) {
    GunEntity::throw_gun(facing_right);
    it_mooving += config.get_server_ticks() / 3;
}

void GrenadeG::start_shooting() {
    GunEntity::start_shooting();
    type = ActiveGrenade;
}

void GrenadeG::stop_shooting() {}

bool GrenadeG::update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) {
    (void)facing_right;
    (void)facing_up;
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
        int16_t angle = 360;
        angle += get_rand_angle();
        bullets->add_bullet(hb, angle % 360, type);
        --ammo;
    }
    destroy();
}

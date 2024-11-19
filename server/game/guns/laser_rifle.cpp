#include "laser_rifle.h"

#include <cstdint>

#include "common/config.h"
#include "server/game/gun_entity.h"

static Config& config = Config::get_instance();

LaserRifleG::LaserRifleG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions):
        GunEntity(gun, bullets, collisions) {
    hitbox.height = LASER_RIFLE_HITBOX_HEIGHT;
    hitbox.width = LASER_RIFLE_HITBOX_WIDTH;
    bullets_to_shoot = 1;
    it_to_shoot = 10;
    it_since_shoot = it_to_shoot;
    initial_angle = 45;
}


bool LaserRifleG::update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) {
    if (ammo > 0) {
        if (trigger_pulled && !it_since_shoot) {
            shooted_bullets = bullets_to_shoot;
        }
        int16_t angle = calculate_initial_angle(facing_right, facing_up) + 360;
        if (angle % 360 < 90) {
            angle -= initial_angle;
        } else {
            angle += initial_angle;
        }
        angle += get_rand_angle();
        add_bullet(player_hb, angle % 360, facing_right, facing_up);
        if (trigger_pulled) {
            ++inaccuracy;
        }
    }
    return false;
}

void LaserRifleG::stop_shooting() {
    GunEntity::stop_shooting();
    inaccuracy = config.get_gun_recoil(type);
}

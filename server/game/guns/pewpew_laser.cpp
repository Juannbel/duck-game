#include "pewpew_laser.h"

#include <cstdint>

#include "common/config.h"

static Config& config = Config::get_instance();

const int16_t DISPERSION = 6;

PewPewLaserG::PewPewLaserG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions):
        GunEntity(gun, bullets, collisions) {
    hitbox.height = PEWPEW_LASER_HITBOX_HEIGHT;
    hitbox.width = PEWPEW_LASER_HITBOX_WIDTH;
    it_to_shoot = config.get_server_ticks() / 20;
    initial_angle = 0;
    bullets_to_shoot = 3;
}

bool PewPewLaserG::update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) {
    if (ammo > 0) {
        int16_t angle = calculate_initial_angle(facing_right, facing_up);
        if (shooted_bullets < 3) {
            int16_t calc_dispersion = shooted_bullets == 1 ? get_rand_angle() + DISPERSION :
                                                             get_rand_angle() - DISPERSION;
            angle += 360;
            angle += calc_dispersion;
            angle %= 360;
        }
        add_bullet(player_hb, angle, facing_right, facing_up);
    }
    return false;
}

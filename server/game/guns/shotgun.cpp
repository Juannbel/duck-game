#include "shotgun.h"

#include <cstdint>

ShootgunG::ShootgunG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions):
        GunEntity(gun, bullets, collisions) {
    hitbox.height = SHOTGUN_HITBOX_HEIGHT;
    hitbox.width = SHOTGUN_HITBOX_WIDTH;
    damage = 50;
    ammo = 2 * 6;
    it_to_shoot = 0;
    initial_angle = 0;
    inaccuracy = 30;
    bullets_to_shoot = 6;
    it_reloading = 1;
    range = 8 * BLOCK_SIZE;
}

bool ShootgunG::update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) {
    if (ammo > 0) {
        if (!it_reloading && shooted_bullets) {
            it_reloading = 1;
            trigger_pulled = false;
            shooted_bullets = 0;
            return false;
        }
        uint8_t aux = ammo;
        for (int i = 0; i < 6; ++i) {
            int16_t angle =
                    calculate_initial_angle(facing_right, facing_up) + 360 + get_rand_angle();
            add_bullet(player_hb, angle % 360, facing_right, facing_up);
        }
        if (aux > ammo) {
            it_reloading = 0;
            trigger_pulled = false;
        }
        return aux > ammo;
    }
    return false;
}

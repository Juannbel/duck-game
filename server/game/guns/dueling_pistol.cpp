#include "dueling_pistol.h"
#include <cstdint>

DuelingPistolG::DuelingPistolG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions):
        GunEntity(gun, bullets, collisions) {
    hitbox.height = DUELING_PISTOL_HITBOX_HEIGHT;
    hitbox.width = DUELING_PISTOL_HITBOX_WIDTH;
    ammo = 1;
    bullets_to_shoot = 1;
    it_to_shoot = 0;
    initial_angle = 0;
    inaccuracy = 8;
    range = 5 * BLOCK_SIZE;
}

bool DuelingPistolG::update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) {
    if (ammo > 0) {
        int16_t angle = calculate_initial_angle(facing_right, facing_up) + 360 + get_rand_angle();
        add_bullet(player_hb, angle % 360, facing_right, facing_up);
    }
    return false;
}

#include "magnum.h"

MagnumG::MagnumG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions):
        GunEntity(gun, bullets, collisions) {
    hitbox.height = MAGNUM_HITBOX_HEIGHT;
    hitbox.width = MAGNUM_HITBOX_WIDTH;
    damage = 55;
    ammo = 6;
    bullets_to_shoot = 1;
    it_to_shoot = 0;
    initial_angle = 0;
    inaccuracy = 8;
    range = 20 * BLOCK_SIZE;
}

// void MagnumG::start_shooting() {}
// void MagnumG::stop_shooting() {}
bool MagnumG::update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) {
    if (ammo > 0) {
        uint8_t aux = ammo;
        int16_t angle = calculate_initial_angle(facing_right, facing_up) + 360 + get_rand_angle();
        add_bullet(player_hb, angle % 360, facing_right, facing_up);
        return aux > ammo;
    }
    return false;
}

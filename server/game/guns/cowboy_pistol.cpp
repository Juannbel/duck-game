#include "cowboy_pistol.h"

CowboyPistolG::CowboyPistolG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions):
        GunEntity(gun, bullets, collisions) {
    hitbox.height = COWBOY_PISTOL_HITBOX_HEIGHT;
    hitbox.width = COWBOY_PISTOL_HITBOX_WIDTH;
    damage = 35;
    ammo = 6;
    bullets_to_shoot = 1;
    it_to_shoot = 0;
    initial_angle = 0;
    it_reloading = 1;
    inaccuracy = 0;
    range = 20 * BLOCK_SIZE;
}

bool CowboyPistolG::update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) {
    if (ammo > 0) {
        add_bullet(player_hb, calculate_initial_angle(facing_right, facing_up), facing_right, facing_up);
    }
    return false;
}

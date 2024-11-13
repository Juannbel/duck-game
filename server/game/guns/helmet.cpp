#include "helmet.h"

HelmetG::HelmetG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions):
        GunEntity(gun, bullets, collisions) {
    hitbox.height = HELMET_HITBOX_HEIGHT;
    hitbox.width = HELMET_HITBOX_WIDTH;
    ammo = 1;
    bullets_to_shoot = 1;
}

bool HelmetG::update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) {
    (void)player_hb;
    (void)facing_right;
    (void)facing_up;
    if (shooted_bullets) {
        return true;
    }
    return false;
}

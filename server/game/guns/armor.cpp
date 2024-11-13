#include "armor.h"

ArmorG::ArmorG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions):
        GunEntity(gun, bullets, collisions) {
    hitbox.height = ARMOR_HITBOX_HEIGHT;
    hitbox.width = ARMOR_HITBOX_WIDTH;
    ammo = 1;
    bullets_to_shoot = 1;
}

bool ArmorG::update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) {
    (void)player_hb;
    (void)facing_right;
    (void)facing_up;
    if (shooted_bullets) {
        return true;
    }
    return false;
}

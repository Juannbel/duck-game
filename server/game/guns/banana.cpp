#include "banana.h"

#include "server/game/gun_entity.h"
#include "server/game/ticks.h"

BananaG::BananaG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions):
        GunEntity(gun, bullets, collisions) {
    hitbox.height = BANANA_HITBOX_HEIGHT;
    hitbox.width = BANANA_HITBOX_WIDTH;
    ammo = 1;
    it_to_shoot = 0;
    range = TICKS;
}

void BananaG::start_shooting() {
    GunEntity::start_shooting();
    type = ActiveBanana;
}

void BananaG::stop_shooting() {}

bool BananaG::update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) { 
    (void) facing_right;
    (void) facing_up;
    hitbox.coords = player_hb.coords;
    return false;
}

void BananaG::throw_gun(bool facing_right) {
    GunEntity::throw_gun(facing_right);
    if (type == ActiveBanana) {
        Rectangle b_hitbox = hitbox;
        b_hitbox.height = BULLET_HITBOX_HEIGHT;
        b_hitbox.width = BULLET_HITBOX_WIDTH;
        b_hitbox.coords.x = facing_right ? b_hitbox.coords.x + DUCK_HITBOX_WIDTH + 2 :
                                           b_hitbox.coords.x - BULLET_HITBOX_WIDTH - 2;
        int16_t angle = facing_right ? 0 : 180;
        type = Banana;
        bullets->add_bullet(b_hitbox, angle, type, range);
        destroy();
    }
}

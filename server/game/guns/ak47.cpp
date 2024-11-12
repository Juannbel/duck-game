#include "ak47.h"
#include <cstdint>
#include "server/game/gun_entity.h"
#include "server/game/ticks.h"

const int16_t INITIAL_INNACURACY = 10;

Ak47G::Ak47G(Gun& gun, BulletManager* bullets, CollisionChecks& collisions):
        GunEntity(gun, bullets, collisions) {
    hitbox.height = AK47_HITBOX_HEIGHT;
    hitbox.width = AK47_HITBOX_WIDTH;
    ammo = 30;
    bullets_to_shoot = 1;
    it_to_shoot = TICKS/15;
    it_since_shoot = it_to_shoot;
    initial_angle = 0;
    inaccuracy = INITIAL_INNACURACY;
    range = 13 * BLOCK_SIZE;
}

bool Ak47G::update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) {
    if (ammo > 0) {
        uint8_t aux = ammo;
        if (trigger_pulled && !it_since_shoot) {
            shooted_bullets = bullets_to_shoot;
            ++inaccuracy;
        }
        int16_t angle = calculate_initial_angle(facing_right, facing_up) + get_rand_angle();
        add_bullet(player_hb, angle, facing_up);
        return aux > ammo;
    }
    return false;
}

void Ak47G::stop_shooting() {
    GunEntity::stop_shooting();
    inaccuracy = INITIAL_INNACURACY;
}

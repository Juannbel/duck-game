#include "ak47.h"

#include <cstdint>

#include "server/game/gun_entity.h"
#include "common/config.h"

static Config& config = Config::get_instance();

const int16_t MAX_INNACURACY = 30;

Ak47G::Ak47G(Gun& gun, BulletManager* bullets, CollisionChecks& collisions):
        GunEntity(gun, bullets, collisions) {
    hitbox.height = AK47_HITBOX_HEIGHT;
    hitbox.width = AK47_HITBOX_WIDTH;
    bullets_to_shoot = 1;
    it_to_shoot = config.get_server_ticks() / 15;
    it_since_shoot = it_to_shoot;
    initial_angle = 0;
}

bool Ak47G::update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) {
    if (ammo > 0) {
        uint8_t aux = ammo;
        if (trigger_pulled && !it_since_shoot) {
            shooted_bullets = bullets_to_shoot;
        }
        int16_t angle = calculate_initial_angle(facing_right, facing_up) + 360 + get_rand_angle();
        add_bullet(player_hb, angle % 360, facing_right, facing_up);
        if (aux > ammo) {
            inaccuracy = inaccuracy > MAX_INNACURACY ? inaccuracy : inaccuracy + 2;
        }
        return aux > ammo;
    }
    return false;
}

void Ak47G::stop_shooting() {
    GunEntity::stop_shooting();
    inaccuracy = config.get_gun_recoil(type);
}

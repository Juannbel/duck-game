#include "sniper.h"

#include "server/game/ticks.h"


SniperG::SniperG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions):
        GunEntity(gun, bullets, collisions) {
    hitbox.height = SNIPER_HITBOX_HEIGHT;
    hitbox.width = SNIPER_HITBOX_WIDTH;
    damage = 85;
    ammo = 3;
    bullets_to_shoot = 1;
    inaccuracy = 0;
    it_reloading = 0;
    range = 64 * BLOCK_SIZE;
}

bool SniperG::update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) {
    if (ammo > 0) {
        if (!it_reloading && shooted_bullets) {
            add_bullet(player_hb, calculate_initial_angle(facing_right, facing_up), facing_right,
                       facing_up);
            it_reloading = TICKS / 2;
            trigger_pulled = false;
        } else {
            it_reloading = !it_reloading ? 0 : it_reloading - 1;
            shooted_bullets = 0;
        }
    }
    return false;
}

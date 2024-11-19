#include "sniper.h"

#include "common/config.h"

static Config& config = Config::get_instance();

SniperG::SniperG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions):
        GunEntity(gun, bullets, collisions) {
    hitbox.height = SNIPER_HITBOX_HEIGHT;
    hitbox.width = SNIPER_HITBOX_WIDTH;
    bullets_to_shoot = 1;
    it_reloading = 0;
}

bool SniperG::update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) {
    if (ammo > 0) {
        if (!it_reloading && shooted_bullets) {
            add_bullet(player_hb, calculate_initial_angle(facing_right, facing_up), facing_right,
                       facing_up);
            it_reloading = config.get_server_ticks() / 2;
            trigger_pulled = false;
        } else {
            it_reloading = !it_reloading ? 0 : it_reloading - 1;
            shooted_bullets = 0;
        }
    }
    return false;
}

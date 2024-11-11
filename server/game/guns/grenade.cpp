//#include "grenade.h"
//#include "server/game/ticks.h"

// GrenadeG::GrenadeG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions):
//         GunEntity(gun, bullets, collisions) {
//     ammo = 30;
//     it_to_shoot = TICKS * 4;
//     inaccuracy = 360;
//     range = 5 * BLOCK_SIZE;
// }
// 
// void GrenadeG::stop_shooting() {}
// void GrenadeG::update_bullets(DuckPlayer& player) {
//     hitbox.coords = player.get_coords();
//     if (it_since_shoot == it_to_shoot) {
//         explode_grenade();
//         player.drop_collectable();
//     }
//     if (trigger_pulled && it_since_shoot < it_to_shoot) {
//         ++it_since_shoot;
//     }
// }
// void GrenadeG::update_status() {
//     check_movement();
//     if (type == Grenade) {
//         if (it_since_shoot == it_to_shoot) {
//             explode_grenade();
//         } else if (trigger_pulled) {
//             ++it_since_shoot;
//         }
//     }
// }
// 
// void GrenadeG::explode_grenade() {
//     Rectangle hb = hitbox;
//     hb.height = BULLET_HITBOX_HEIGHT;
//     hb.width = BULLET_HITBOX_WIDTH;
//     for (int i = 0; i < 30; ++i) {
//         int16_t angle = 0;
//         angle += get_rand_angle();
//         bullets->add_bullet(hb, angle, type, range);
//     }
//     destroy();
// }
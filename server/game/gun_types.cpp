//#include "gun_types.h"
//
//#include <cstdint>
//
//#include "common/shared_constants.h"
//#include "common/snapshot.h"
//#include "server/game/gun_entity.h"
//
//#include "ticks.h"
//
//const uint8_t AK_CD = TICKS / 6;
//
//GrenadeG::GrenadeG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions, bool explode):
//        GunEntity(gun, bullets, collisions) {
//    hitbox.height = GRENADE_HITBOX_HEIGHT;
//    hitbox.width = GRENADE_HITBOX_WIDTH;
//    ammo = 30;
//    it_to_shoot = TICKS * 4;
//    inaccuracy = 360;
//    range = 5 * BLOCK_SIZE;
//    if (explode) {
//        explode_grenade();
//    }
//}
//
//void GrenadeG::start_shooting() {
//    GunEntity::start_shooting();
//    type = ActiveGrenade;
//}
//
//void GrenadeG::stop_shooting() {}
//
//bool GrenadeG::update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) {
//    (void) facing_right;
//    (void) facing_up;
//    hitbox.coords = player_hb.coords;
//    if (it_since_shoot == it_to_shoot) {
//        explode_grenade();
//    }
//    if (trigger_pulled && it_since_shoot < it_to_shoot) {
//        ++it_since_shoot;
//    }
//    return false;
//}
//
//void GrenadeG::update_status() {
//    check_movement();
//    if (type == ActiveGrenade) {
//        if (it_since_shoot == it_to_shoot) {
//            explode_grenade();
//        } else if (trigger_pulled) {
//            ++it_since_shoot;
//        }
//    }
//}
//
//void GrenadeG::explode_grenade() {
//    Rectangle hb = hitbox;
//    hb.height = BULLET_HITBOX_HEIGHT;
//    hb.width = BULLET_HITBOX_WIDTH;
//    type = Grenade;
//    while (ammo > 0) {
//        int16_t angle = 0;
//        angle += get_rand_angle();
//        bullets->add_bullet(hb, angle, type, range);
//        --ammo;
//    }
//    destroy();
//}
//
//BananaG::BananaG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions):
//        GunEntity(gun, bullets, collisions) {
//    hitbox.height = BANANA_HITBOX_HEIGHT;
//    hitbox.width = BANANA_HITBOX_WIDTH;
//    ammo = 1;
//    it_to_shoot = 0;
//    range = TICKS;
//}
//
//void BananaG::start_shooting() {
//    GunEntity::start_shooting();
//    type = ActiveBanana;
//}
//
//void BananaG::stop_shooting() {}
//
//bool BananaG::update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) { 
//    (void) facing_right;
//    (void) facing_up;
//    hitbox.coords = player_hb.coords;
//    return false;
//}
//
//void BananaG::throw_gun(bool facing_right) {
//    this->facing_right = facing_right;
//    it_mooving = TICKS / 2;
//    if (type == ActiveBanana) {
//        Rectangle b_hitbox = hitbox;
//        b_hitbox.height = BULLET_HITBOX_HEIGHT;
//        b_hitbox.width = BULLET_HITBOX_WIDTH;
//        b_hitbox.coords.x = facing_right ? b_hitbox.coords.x + DUCK_HITBOX_WIDTH + 2 :
//                                           b_hitbox.coords.x - BULLET_HITBOX_WIDTH - 2;
//        int16_t angle = facing_right ? 0 : 180;
//        type = Banana;
//        bullets->add_bullet(b_hitbox, angle, type, range);
//        destroy();
//    }
//}
//
//PewPewLaserG::PewPewLaserG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions):
//        GunEntity(gun, bullets, collisions) {
//    hitbox.height = PEWPEW_LASER_HITBOX_HEIGHT;
//    hitbox.width = PEWPEW_LASER_HITBOX_WIDTH;
//    ammo = 12 * 3;
//    it_to_shoot = TICKS/20;
//    initial_angle = 0;
//    inaccuracy = 10;
//    bullets_to_shoot = 3;
//    range = 35 * BLOCK_SIZE;
//}
//
//// void PewPewLaserG::start_shooting() { trigger_pulled = true; }
//// void PewPewLaserG::stop_shooting() { trigger_pulled = false; }
//bool PewPewLaserG::update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) {
//    if (ammo > 0) {
//        add_bullet(player_hb, facing_right, facing_up);
//    }
//    return false;
//}
//
//
//LaserRifleG::LaserRifleG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions):
//        GunEntity(gun, bullets, collisions) {
//    hitbox.height = LASER_RIFLE_HITBOX_HEIGHT;
//    hitbox.width = LASER_RIFLE_HITBOX_WIDTH;
//    ammo = 10;
//    bullets_to_shoot = 1;
//    it_to_shoot = 10;
//    it_since_shoot = it_to_shoot;
//    initial_angle = 0;
//    inaccuracy = 10;
//    range = 30 * BLOCK_SIZE;
//}
//
//
//// void LaserRifleG::start_shooting() {}
//bool LaserRifleG::update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) {
//    if (ammo > 0) {
//        if (trigger_pulled && !it_since_shoot) {
//            shooted_bullets = bullets_to_shoot;
//        }
//        add_bullet(player_hb, facing_right,facing_up);
//    }
//    return false;
//}
//// void LaserRifleG::stop_shooting() {}
//
//
//Ak47G::Ak47G(Gun& gun, BulletManager* bullets, CollisionChecks& collisions):
//        GunEntity(gun, bullets, collisions) {
//    hitbox.height = AK47_HITBOX_HEIGHT;
//    hitbox.width = AK47_HITBOX_WIDTH;
//    ammo = 30;
//    bullets_to_shoot = 1;
//    it_to_shoot = TICKS/15;
//    it_since_shoot = it_to_shoot;
//    initial_angle = 0;
//    inaccuracy = 15;
//    range = 13 * BLOCK_SIZE;
//}
//
//// void Ak47G::start_shooting() {
////     it_since_shoot = trigger_pulled ? it_since_shoot : 0;
////     trigger_pulled = true;
//// }
//// void Ak47G::stop_shooting() {
////     trigger_pulled = false;
//// }
//
//bool Ak47G::update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) {
//    if (ammo > 0) {
//        uint8_t aux = ammo;
//        if (trigger_pulled && !it_since_shoot) {
//            shooted_bullets = bullets_to_shoot;
//        }
//        add_bullet(player_hb, facing_right,facing_up);
//        return aux > ammo;
//    }
//    return false;
//}
//
//
//DuelingPistolG::DuelingPistolG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions):
//        GunEntity(gun, bullets, collisions) {
//    hitbox.height = DUELING_PISTOL_HITBOX_HEIGHT;
//    hitbox.width = DUELING_PISTOL_HITBOX_WIDTH;
//    ammo = 1;
//    bullets_to_shoot = 1;
//    it_to_shoot = 0;
//    initial_angle = 0;
//    inaccuracy = 8;
//    range = 5 * BLOCK_SIZE;
//}
//
//// void DuelingPistolG::start_shooting() {}
//// void DuelingPistolG::stop_shooting() {}
//bool DuelingPistolG::update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) {
//    if (ammo > 0) {
//        add_bullet(player_hb, facing_right,facing_up);
//    }
//    return false;
//}
//
//
//CowboyPistolG::CowboyPistolG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions):
//        GunEntity(gun, bullets, collisions) {
//    hitbox.height = COWBOY_PISTOL_HITBOX_HEIGHT;
//    hitbox.width = COWBOY_PISTOL_HITBOX_WIDTH;
//    ammo = 6;
//    bullets_to_shoot = 1;
//    it_to_shoot = 0;
//    initial_angle = 0;
//    inaccuracy = 0;
//    range = 20 * BLOCK_SIZE;
//}
//
//// void CowboyPistolG::start_shooting() {}
//// void CowboyPistolG::stop_shooting() {}
//bool CowboyPistolG::update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) {
//    if (ammo > 0) {
//        add_bullet(player_hb, facing_right,facing_up);
//    }
//    return false;
//}
//
//MagnumG::MagnumG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions):
//        GunEntity(gun, bullets, collisions) {
//    hitbox.height = MAGNUM_HITBOX_HEIGHT;
//    hitbox.width = MAGNUM_HITBOX_WIDTH;
//    ammo = 6;
//    bullets_to_shoot = 1;
//    it_to_shoot = 0;
//    initial_angle = 0;
//    inaccuracy = 10;
//    range = 20 * BLOCK_SIZE;
//}
//
//// void MagnumG::start_shooting() {}
//// void MagnumG::stop_shooting() {}
//bool MagnumG::update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) {
//    if (ammo > 0) {
//        uint8_t aux = ammo;
//        add_bullet(player_hb, facing_right,facing_up);
//        return aux > ammo;
//    }
//    return false;
//}
//
//ShootgunG::ShootgunG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions):
//        GunEntity(gun, bullets, collisions) {
//    hitbox.height = SHOTGUN_HITBOX_HEIGHT;
//    hitbox.width = SHOTGUN_HITBOX_WIDTH;
//    ammo = 2 * 6;
//    it_to_shoot = 0;
//    initial_angle = 0;
//    inaccuracy = 30;
//    bullets_to_shoot = 6;
//    it_reloading = 1;
//    range = 8 * BLOCK_SIZE;
//}
//
//// void ShootgunG::start_shooting() {}
//// void ShootgunG::stop_shooting() {}
//bool ShootgunG::update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) {
//    if (ammo > 0) {
//        if (!it_reloading && shooted_bullets) {
//            it_reloading = 1;
//            trigger_pulled = false;
//            shooted_bullets = 0;
//            return false;
//        }
//        uint8_t aux = ammo;
//        for (int i = 0; i < 6; ++i) {
//            add_bullet(player_hb, facing_right,facing_up);
//        }
//        if (aux > ammo) {
//            it_reloading = 0;
//            trigger_pulled = false;
//        }
//        return aux > ammo;
//    }
//    return false;
//}
//
//SniperG::SniperG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions):
//        GunEntity(gun, bullets, collisions) {
//    hitbox.height = SNIPER_HITBOX_HEIGHT;
//    hitbox.width = SNIPER_HITBOX_WIDTH;
//    ammo = 3;
//    bullets_to_shoot = 1;
//    inaccuracy = 0;
//    it_reloading = 0;
//    range = 64 * BLOCK_SIZE;
//}
//
//// void SniperG::start_shooting() {}
//// void SniperG::stop_shooting() {}
//bool SniperG::update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) {
//    if (ammo > 0) {
//        if (!it_reloading && shooted_bullets) {
//            add_bullet(player_hb, facing_right,facing_up);
//            it_reloading = TICKS/2;
//            trigger_pulled = false;
//        } else {
//            it_reloading = !it_reloading ? 0 : it_reloading-1;
//            shooted_bullets = 0;
//        }
//    }
//    return false;
//}
//
//HelmetG::HelmetG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions):
//        GunEntity(gun, bullets, collisions) {
//    hitbox.height = HELMET_HITBOX_HEIGHT;
//    hitbox.width = HELMET_HITBOX_WIDTH;
//    ammo = 1;
//    bullets_to_shoot = 1;
//}
//
//// void HelmetG::start_shooting() {}
//// void HelmetG::stop_shooting() {}
//bool HelmetG::update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) {
//    (void) player_hb;
//    (void) facing_right;
//    (void) facing_up;
//    if (shooted_bullets) {
//        return true;
//    }
//    return false;
//}
//
//ArmorG::ArmorG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions):
//        GunEntity(gun, bullets, collisions) {
//    hitbox.height = HELMET_HITBOX_HEIGHT;
//    hitbox.width = HELMET_HITBOX_WIDTH;
//    ammo = 1;
//    bullets_to_shoot = 1;
//}
//
//// void ArmorG::start_shooting() {}
//// void ArmorG::stop_shooting() {}
//bool ArmorG::update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) {
//    (void) player_hb;
//    (void) facing_right;
//    (void) facing_up;
//    if (shooted_bullets) {
//        return true;
//    }
//    return false;
//}
//
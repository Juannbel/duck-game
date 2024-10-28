#include "gun_types.h"
#include "ticks.h"

const uint8_t AK_CD = TICKS/6;

GrenadeG::GrenadeG(Gun& gun, CollisionChecks& collisions, BulletManager* bullets) : GunEntity(gun, collisions, bullets) {
    ammo = 2;
}

void GrenadeG::start_shooting() {}
void GrenadeG::stop_shooting() {}
void GrenadeG::update_bullets(const Duck& status) {

}

BananaG::BananaG(Gun& gun, CollisionChecks& collisions, BulletManager* bullets) : GunEntity(gun, collisions, bullets) {
    ammo = 2;
}

void BananaG::start_shooting() {}
void BananaG::stop_shooting() {}
void BananaG::update_bullets(const Duck& status) {

}

PewPewLaserG::PewPewLaserG(Gun& gun, CollisionChecks& collisions, BulletManager* bullets) : GunEntity(gun, collisions, bullets) {
    ammo = 12*3;
}

void PewPewLaserG::start_shooting() {}
void PewPewLaserG::stop_shooting() {}
void PewPewLaserG::update_bullets(const Duck& status) {

}


LaserRifleG::LaserRifleG(Gun& gun, CollisionChecks& collisions, BulletManager* bullets) : GunEntity(gun, collisions, bullets) {
    ammo = 10;
}

void LaserRifleG::start_shooting() {}
void LaserRifleG::stop_shooting() {}
void LaserRifleG::update_bullets(const Duck& status) {

}


Ak47G::Ak47G(Gun& gun, CollisionChecks& collisions, BulletManager* bullets) : GunEntity(gun, collisions, bullets) {
    ammo = 30;
}

void Ak47G::start_shooting() {
    it_since_shoot = trigger_pulled ? it_since_shoot : 0;
    trigger_pulled = true;
}
void Ak47G::stop_shooting() {
    trigger_pulled = false;
}

void Ak47G::update_bullets(const Duck& status) {
    if (trigger_pulled && it_since_shoot > AK_CD) {
        int16_t x = status.facing_right ? status.x+DUCK_HITBOX_WIDTH : status.x;
        int16_t y = status.y+DUCK_LAYED_HITBOX_HEIGHT;
        uint16_t angle = status.facing_right ? 0 : 180;
        Rectangle hitbox;
        hitbox.coords.x = x;
        hitbox.coords.y = y;
        hitbox.height = BULLET_HITBOX_HEIGHT;
        hitbox.width = BULLET_HITBOX_WIDTH;
        Bullet status = {0, x, y, angle, type};
        BulletInfo bullet = {status, hitbox, 10};
        bullets->add_bullet(bullet);
        it_since_shoot = 0;
    }
    ++it_since_shoot;
}


DuelingPistolG::DuelingPistolG(Gun& gun, CollisionChecks& collisions, BulletManager* bullets) : GunEntity(gun, collisions, bullets) {
    ammo = 1;
}

void DuelingPistolG::start_shooting() {}
void DuelingPistolG::stop_shooting() {}
void DuelingPistolG::update_bullets(const Duck& status) {

}


CowboyPistolG::CowboyPistolG(Gun& gun, CollisionChecks& collisions, BulletManager* bullets) : GunEntity(gun, collisions, bullets) {
    ammo = 6;
}

void CowboyPistolG::start_shooting() {}
void CowboyPistolG::stop_shooting() {}
void CowboyPistolG::update_bullets(const Duck& status) {

}

MagnumG::MagnumG(Gun& gun, CollisionChecks& collisions, BulletManager* bullets) : GunEntity(gun, collisions, bullets) {
    ammo = 6;
}

void MagnumG::start_shooting() {}
void MagnumG::stop_shooting() {}
void MagnumG::update_bullets(const Duck& status) {

}

ShootgunG::ShootgunG(Gun& gun, CollisionChecks& collisions, BulletManager* bullets) : GunEntity(gun, collisions, bullets) {
    ammo = 2;
}

void ShootgunG::start_shooting() {}
void ShootgunG::stop_shooting() {}
void ShootgunG::update_bullets(const Duck& status) {

}

SniperG::SniperG(Gun& gun, CollisionChecks& collisions, BulletManager* bullets) : GunEntity(gun, collisions, bullets) {
    ammo = 3;
}

void SniperG::start_shooting() {}
void SniperG::stop_shooting() {}
void SniperG::update_bullets(const Duck& status) {

}

HelmetG::HelmetG(Gun& gun, CollisionChecks& collisions, BulletManager* bullets) : GunEntity(gun, collisions, bullets) {
    ammo = 1;
}

void HelmetG::start_shooting() {}
void HelmetG::stop_shooting() {}
void HelmetG::update_bullets(const Duck& status) {

}

ArmorG::ArmorG(Gun& gun, CollisionChecks& collisions, BulletManager* bullets) : GunEntity(gun, collisions, bullets) {
    ammo = 1;
}

void ArmorG::start_shooting() {}
void ArmorG::stop_shooting() {}
void ArmorG::update_bullets(const Duck& status) {

}

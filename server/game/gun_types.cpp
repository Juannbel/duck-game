#include "gun_types.h"

#include <cstdint>

#include "duck_player.h"
#include "ticks.h"

const uint8_t AK_CD = TICKS / 6;

GrenadeG::GrenadeG(Gun& gun, BulletManager* bullets): GunEntity(gun, bullets) { ammo = 2; }

// void GrenadeG::start_shooting() {}
// void GrenadeG::stop_shooting() {}
void GrenadeG::update_bullets(DuckPlayer& player) {
    if (trigger_pulled) {
        Duck status = player.get_status();
        int16_t x = status.facing_right ? status.x + DUCK_HITBOX_WIDTH : status.x;
        int16_t y = status.y + DUCK_LAYED_HITBOX_HEIGHT;
        int16_t angle = status.facing_right ? 0 : 180;
        int16_t max = 20;
        int16_t min = -20;
        int16_t randNum = rand() % (max - min + 1) + min;
        angle += randNum;
        Rectangle hitbox;
        hitbox.coords.x = x;
        hitbox.coords.y = y;
        hitbox.height = BULLET_HITBOX_HEIGHT;
        hitbox.width = BULLET_HITBOX_WIDTH;
        Bullet bullet_status = {0, x, y, static_cast<uint16_t>(angle % 360), type};
        BulletInfo bullet = {bullet_status, hitbox, 3};
        bullets->add_bullet(bullet);
        it_since_shoot = 0;
    }
    ++it_since_shoot;
}

BananaG::BananaG(Gun& gun, BulletManager* bullets): GunEntity(gun, bullets) { ammo = 2; }

// void BananaG::start_shooting() {}
// void BananaG::stop_shooting() {}
void BananaG::update_bullets(DuckPlayer& player) {
    if (trigger_pulled) {
        Duck status = player.get_status();
        int16_t x = status.facing_right ? status.x + DUCK_HITBOX_WIDTH : status.x;
        int16_t y = status.y + DUCK_LAYED_HITBOX_HEIGHT;
        uint16_t angle = status.facing_right ? 0 : 180;
        Rectangle hitbox;
        hitbox.coords.x = x;
        hitbox.coords.y = y;
        hitbox.height = BULLET_HITBOX_HEIGHT;
        hitbox.width = BULLET_HITBOX_WIDTH;
        Bullet bullet_status = {0, x, y, angle, type};
        BulletInfo bullet = {bullet_status, hitbox, 3};
        bullets->add_bullet(bullet);
        it_since_shoot = 0;
    }
    ++it_since_shoot;
}

PewPewLaserG::PewPewLaserG(Gun& gun, BulletManager* bullets): GunEntity(gun, bullets) {
    ammo = 12 * 3;
}

// void PewPewLaserG::start_shooting() { trigger_pulled = true; }
// void PewPewLaserG::stop_shooting() { trigger_pulled = false; }
void PewPewLaserG::update_bullets(DuckPlayer& player) {
    if (trigger_pulled) {
        Duck status = player.get_status();

        int16_t x = status.facing_right ? status.x + DUCK_HITBOX_WIDTH : status.x;
        int16_t y = status.y + DUCK_LAYED_HITBOX_HEIGHT;
        int16_t angle = status.facing_right ? 0 : 180;
        int16_t max = 20;
        int16_t min = -20;
        int16_t randNum = rand() % (max - min + 1) + min;
        angle += randNum;
        Rectangle hitbox;
        hitbox.coords.x = x;
        hitbox.coords.y = y;
        hitbox.height = BULLET_HITBOX_HEIGHT;
        hitbox.width = BULLET_HITBOX_WIDTH;
        Bullet bullet_status = {0, x, y, static_cast<uint16_t>(angle % 360), type};
        BulletInfo bullet = {bullet_status, hitbox, 3};
        bullets->add_bullet(bullet);
        it_since_shoot = 0;
    }
    ++it_since_shoot;
}


LaserRifleG::LaserRifleG(Gun& gun, BulletManager* bullets): GunEntity(gun, bullets) { ammo = 10; }

// void LaserRifleG::start_shooting() {}
// void LaserRifleG::stop_shooting() {}
void LaserRifleG::update_bullets(DuckPlayer& player) {
    if (trigger_pulled) {
        Duck status = player.get_status();
        int16_t x = status.facing_right ? status.x + DUCK_HITBOX_WIDTH : status.x;
        int16_t y = status.y + DUCK_LAYED_HITBOX_HEIGHT;
        int16_t angle = status.facing_right ? 0 : 180;
        int16_t max = 20;
        int16_t min = -20;
        int16_t randNum = rand() % (max - min + 1) + min;
        angle += randNum;
        Rectangle hitbox;
        hitbox.coords.x = x;
        hitbox.coords.y = y;
        hitbox.height = BULLET_HITBOX_HEIGHT;
        hitbox.width = BULLET_HITBOX_WIDTH;
        Bullet bullet_status = {0, x, y, static_cast<uint16_t>(angle % 360), type};
        BulletInfo bullet = {bullet_status, hitbox, 3};
        bullets->add_bullet(bullet);
        it_since_shoot = 0;
    }
    ++it_since_shoot;
}


Ak47G::Ak47G(Gun& gun, BulletManager* bullets): GunEntity(gun, bullets) { ammo = 30; }

// void Ak47G::start_shooting() {
//     it_since_shoot = trigger_pulled ? it_since_shoot : 0;
//     trigger_pulled = true;
// }
// void Ak47G::stop_shooting() {
//     trigger_pulled = false;
// }

void Ak47G::update_bullets(DuckPlayer& player) {
    if (trigger_pulled) {
        Duck status = player.get_status();
        int16_t x = status.facing_right ? status.x + DUCK_HITBOX_WIDTH : status.x;
        int16_t y = status.y + DUCK_LAYED_HITBOX_HEIGHT;
        int16_t angle = status.facing_right ? 0 : 180;
        int16_t max = 20;
        int16_t min = -20;
        int16_t randNum = rand() % (max - min + 1) + min;
        angle += randNum;
        Rectangle hitbox;
        hitbox.coords.x = x;
        hitbox.coords.y = y;
        hitbox.height = BULLET_HITBOX_HEIGHT;
        hitbox.width = BULLET_HITBOX_WIDTH;
        Bullet bullet_status = {0, x, y, static_cast<uint16_t>(angle % 360), type};
        BulletInfo bullet = {bullet_status, hitbox, 3};
        bullets->add_bullet(bullet);
        it_since_shoot = 0;
    }
    ++it_since_shoot;
}


DuelingPistolG::DuelingPistolG(Gun& gun, BulletManager* bullets): GunEntity(gun, bullets) {
    ammo = 1;
}

// void DuelingPistolG::start_shooting() {}
// void DuelingPistolG::stop_shooting() {}
void DuelingPistolG::update_bullets(DuckPlayer& player) {
    if (trigger_pulled) {
        Duck status = player.get_status();
        int16_t x = status.facing_right ? status.x + DUCK_HITBOX_WIDTH : status.x;
        int16_t y = status.y + DUCK_LAYED_HITBOX_HEIGHT;
        int16_t angle = status.facing_right ? 0 : 180;
        int16_t max = 20;
        int16_t min = -20;
        int16_t randNum = rand() % (max - min + 1) + min;
        angle += randNum;
        Rectangle hitbox;
        hitbox.coords.x = x;
        hitbox.coords.y = y;
        hitbox.height = BULLET_HITBOX_HEIGHT;
        hitbox.width = BULLET_HITBOX_WIDTH;
        Bullet bullet_status = {0, x, y, static_cast<uint16_t>(angle % 360), type};
        BulletInfo bullet = {bullet_status, hitbox, 3};
        bullets->add_bullet(bullet);
        it_since_shoot = 0;
    }
    ++it_since_shoot;
}


CowboyPistolG::CowboyPistolG(Gun& gun, BulletManager* bullets): GunEntity(gun, bullets) {
    ammo = 6;
}

// void CowboyPistolG::start_shooting() {}
// void CowboyPistolG::stop_shooting() {}
void CowboyPistolG::update_bullets(DuckPlayer& player) {
    if (trigger_pulled) {
        Duck status = player.get_status();
        int16_t x = status.facing_right ? status.x + DUCK_HITBOX_WIDTH : status.x;
        int16_t y = status.y + DUCK_LAYED_HITBOX_HEIGHT;
        uint16_t angle = status.facing_right ? 0 : 180;
        Rectangle hitbox;
        hitbox.coords.x = x;
        hitbox.coords.y = y;
        hitbox.height = BULLET_HITBOX_HEIGHT;
        hitbox.width = BULLET_HITBOX_WIDTH;
        Bullet bullet_status = {0, x, y, angle, type};
        BulletInfo bullet = {bullet_status, hitbox, 3};
        bullets->add_bullet(bullet);
        it_since_shoot = 0;
    }
    ++it_since_shoot;
}

MagnumG::MagnumG(Gun& gun, BulletManager* bullets): GunEntity(gun, bullets) { ammo = 6; }

// void MagnumG::start_shooting() {}
// void MagnumG::stop_shooting() {}
void MagnumG::update_bullets(DuckPlayer& player) {
    if (trigger_pulled) {
        Duck status = player.get_status();
        int16_t x = status.facing_right ? status.x + DUCK_HITBOX_WIDTH : status.x;
        int16_t y = status.y + DUCK_LAYED_HITBOX_HEIGHT;
        int16_t angle = status.facing_right ? 0 : 180;
        int16_t max = 20;
        int16_t min = -20;
        int16_t randNum = rand() % (max - min + 1) + min;
        angle += randNum;
        Rectangle hitbox;
        hitbox.coords.x = x;
        hitbox.coords.y = y;
        hitbox.height = BULLET_HITBOX_HEIGHT;
        hitbox.width = BULLET_HITBOX_WIDTH;
        Bullet bullet_status = {0, x, y, static_cast<uint16_t>(angle % 360), type};
        BulletInfo bullet = {bullet_status, hitbox, 3};
        bullets->add_bullet(bullet);
        it_since_shoot = 0;
    }
    ++it_since_shoot;
}

ShootgunG::ShootgunG(Gun& gun, BulletManager* bullets): GunEntity(gun, bullets) { ammo = 2; }

// void ShootgunG::start_shooting() {}
// void ShootgunG::stop_shooting() {}
void ShootgunG::update_bullets(DuckPlayer& player) {
    if (trigger_pulled) {
        Duck status = player.get_status();
        int16_t x = status.facing_right ? status.x + DUCK_HITBOX_WIDTH : status.x;
        int16_t y = status.y + DUCK_LAYED_HITBOX_HEIGHT;
        int16_t angle = status.facing_right ? 0 : 180;
        int16_t max = 20;
        int16_t min = -20;
        int16_t randNum = rand() % (max - min + 1) + min;
        angle += randNum;
        Rectangle hitbox;
        hitbox.coords.x = x;
        hitbox.coords.y = y;
        hitbox.height = BULLET_HITBOX_HEIGHT;
        hitbox.width = BULLET_HITBOX_WIDTH;
        Bullet bullet_status = {0, x, y, static_cast<uint16_t>(angle % 360), type};
        BulletInfo bullet = {bullet_status, hitbox, 3};
        bullets->add_bullet(bullet);
        it_since_shoot = 0;
    }
    ++it_since_shoot;
}

SniperG::SniperG(Gun& gun, BulletManager* bullets): GunEntity(gun, bullets) { ammo = 3; }

// void SniperG::start_shooting() {}
// void SniperG::stop_shooting() {}
void SniperG::update_bullets(DuckPlayer& player) {
    if (trigger_pulled) {
        Duck status = player.get_status();
        int16_t x = status.facing_right ? status.x + DUCK_HITBOX_WIDTH : status.x;
        int16_t y = status.y + DUCK_LAYED_HITBOX_HEIGHT;
        int16_t angle = status.facing_right ? 0 : 180;
        int16_t max = 20;
        int16_t min = -20;
        int16_t randNum = rand() % (max - min + 1) + min;
        angle += randNum;
        Rectangle hitbox;
        hitbox.coords.x = x;
        hitbox.coords.y = y;
        hitbox.height = BULLET_HITBOX_HEIGHT;
        hitbox.width = BULLET_HITBOX_WIDTH;
        Bullet bullet_status = {0, x, y, static_cast<uint16_t>(angle % 360), type};
        BulletInfo bullet = {bullet_status, hitbox, 3};
        bullets->add_bullet(bullet);
        it_since_shoot = 0;
    }
    ++it_since_shoot;
}

HelmetG::HelmetG(Gun& gun, BulletManager* bullets): GunEntity(gun, bullets) { ammo = 1; }

// void HelmetG::start_shooting() {}
// void HelmetG::stop_shooting() {}
void HelmetG::update_bullets(DuckPlayer& player) {
    if (trigger_pulled) {
        Duck status = player.get_status();
        int16_t x = status.facing_right ? status.x + DUCK_HITBOX_WIDTH : status.x;
        int16_t y = status.y + DUCK_LAYED_HITBOX_HEIGHT;
        int16_t angle = status.facing_right ? 0 : 180;
        int16_t max = 20;
        int16_t min = -20;
        int16_t randNum = rand() % (max - min + 1) + min;
        angle += randNum;
        Rectangle hitbox;
        hitbox.coords.x = x;
        hitbox.coords.y = y;
        hitbox.height = BULLET_HITBOX_HEIGHT;
        hitbox.width = BULLET_HITBOX_WIDTH;
        Bullet bullet_status = {0, x, y, static_cast<uint16_t>(angle % 360), type};
        BulletInfo bullet = {bullet_status, hitbox, 3};
        bullets->add_bullet(bullet);
        it_since_shoot = 0;
    }
    ++it_since_shoot;
}

ArmorG::ArmorG(Gun& gun, BulletManager* bullets): GunEntity(gun, bullets) { ammo = 1; }

// void ArmorG::start_shooting() {}
// void ArmorG::stop_shooting() {}
void ArmorG::update_bullets(DuckPlayer& player) {
    if (trigger_pulled) {
        Duck status = player.get_status();
        int16_t x = status.facing_right ? status.x + DUCK_HITBOX_WIDTH : status.x;
        int16_t y = status.y + DUCK_LAYED_HITBOX_HEIGHT;
        int16_t angle = status.facing_right ? 0 : 180;
        int16_t max = 20;
        int16_t min = -20;
        int16_t randNum = rand() % (max - min + 1) + min;
        angle += randNum;
        Rectangle hitbox;
        hitbox.coords.x = x;
        hitbox.coords.y = y;
        hitbox.height = BULLET_HITBOX_HEIGHT;
        hitbox.width = BULLET_HITBOX_WIDTH;
        Bullet bullet_status = {0, x, y, static_cast<uint16_t>(angle % 360), type};
        BulletInfo bullet = {bullet_status, hitbox, 3};
        bullets->add_bullet(bullet);
        it_since_shoot = 0;
    }
    ++it_since_shoot;
}

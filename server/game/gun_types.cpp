#include "gun_types.h"

#include <cstdint>

#include "duck_player.h"
#include "ticks.h"
#include "common/shared_constants.h"

const uint8_t AK_CD = TICKS / 6;

GrenadeG::GrenadeG(Gun& gun, BulletManager* bullets): GunEntity(gun, bullets) {
    ammo = 2;
    it_to_shoot = 0;
    range = 5 * BLOCK_SIZE;
}

// void GrenadeG::start_shooting() {}
// void GrenadeG::stop_shooting() {}
void GrenadeG::update_bullets(DuckPlayer& player) { add_bullet(player); }

BananaG::BananaG(Gun& gun, BulletManager* bullets): GunEntity(gun, bullets) {
    ammo = 2;
    it_to_shoot = 0;
}

// void BananaG::start_shooting() {}
// void BananaG::stop_shooting() {}
void BananaG::update_bullets(DuckPlayer& player) { add_bullet(player); }

PewPewLaserG::PewPewLaserG(Gun& gun, BulletManager* bullets): GunEntity(gun, bullets) {
    ammo = 12 * 3;
    it_to_shoot = 0;
    initial_angle = 0;
    inaccuracy = 10;
    bullets_to_shoot = 3;
    it_to_reload = TICKS / 2;
    it_reloading = it_to_reload;
    range = 35 * BLOCK_SIZE;
}

// void PewPewLaserG::start_shooting() { trigger_pulled = true; }
// void PewPewLaserG::stop_shooting() { trigger_pulled = false; }
void PewPewLaserG::update_bullets(DuckPlayer& player) {
    if (ammo == 0) {
        player.drop_collectable();
        return;
    }
    add_bullet(player);
}


LaserRifleG::LaserRifleG(Gun& gun, BulletManager* bullets): GunEntity(gun, bullets) {
    ammo = 10;
    it_to_shoot = 10;
    it_since_shoot = it_to_shoot;
    initial_angle = 0;
    inaccuracy = 10;
    it_to_reload = TICKS / 2;
    it_reloading = it_to_reload;
    range = 30 * BLOCK_SIZE;

}


// void LaserRifleG::start_shooting() {}
void LaserRifleG::update_bullets(DuckPlayer& player) {
    if (ammo == 0) {
        player.drop_collectable();
        return;
    }
    add_bullet(player);
}
// void LaserRifleG::stop_shooting() {}


Ak47G::Ak47G(Gun& gun, BulletManager* bullets): GunEntity(gun, bullets) {
    ammo = 30;
    it_to_shoot = 8;
    it_since_shoot = it_to_shoot;
    initial_angle = 0;
    inaccuracy = 15;
    it_to_reload = TICKS / 2;
    it_reloading = it_to_reload;
    range = 13 * BLOCK_SIZE;
}

// void Ak47G::start_shooting() {
//     it_since_shoot = trigger_pulled ? it_since_shoot : 0;
//     trigger_pulled = true;
// }
// void Ak47G::stop_shooting() {
//     trigger_pulled = false;
// }

void Ak47G::update_bullets(DuckPlayer& player) {
    if (ammo == 0) {
        player.drop_collectable();
        return;
    }
    add_bullet(player);
}


DuelingPistolG::DuelingPistolG(Gun& gun, BulletManager* bullets): GunEntity(gun, bullets) {
    ammo = 1;
    it_to_shoot = 0;
    initial_angle = 0;
    inaccuracy = 8;
    it_to_reload = 0;
    it_reloading = it_to_reload;
    range = 5 * BLOCK_SIZE;
}

// void DuelingPistolG::start_shooting() {}
// void DuelingPistolG::stop_shooting() {}
void DuelingPistolG::update_bullets(DuckPlayer& player) {
    if (ammo == 0) {
        player.drop_collectable();
        return;
    }
    add_bullet(player);
}


CowboyPistolG::CowboyPistolG(Gun& gun, BulletManager* bullets): GunEntity(gun, bullets) {
    ammo = 6;
    it_to_shoot = 0;
    initial_angle = 0;
    inaccuracy = 0;
    it_to_reload = 0;
    it_reloading = it_to_reload;
    range = 20 * BLOCK_SIZE;
}

// void CowboyPistolG::start_shooting() {}
// void CowboyPistolG::stop_shooting() {}
void CowboyPistolG::update_bullets(DuckPlayer& player) {
    if (ammo == 0) {
        player.drop_collectable();
        return;
    }
    add_bullet(player);
}

MagnumG::MagnumG(Gun& gun, BulletManager* bullets): GunEntity(gun, bullets) {
    ammo = 6;
    it_to_shoot = 0;
    initial_angle = 0;
    inaccuracy = 10;
    it_to_reload = 0;
    it_reloading = it_to_reload;
    range = 20 * BLOCK_SIZE;
}

// void MagnumG::start_shooting() {}
// void MagnumG::stop_shooting() {}
void MagnumG::update_bullets(DuckPlayer& player) {
    if (ammo == 0) {
        player.drop_collectable();
        return;
    }
    add_bullet(player);
}

ShootgunG::ShootgunG(Gun& gun, BulletManager* bullets): GunEntity(gun, bullets) {
    ammo = 2 * 6;
    it_to_shoot = 0;
    initial_angle = 0;
    inaccuracy = 30;
    bullets_to_shoot = 6;
    it_to_reload = TICKS / 2;
    it_reloading = it_to_reload;
    range = 8 * BLOCK_SIZE;
}

// void ShootgunG::start_shooting() {}
// void ShootgunG::stop_shooting() {}
void ShootgunG::update_bullets(DuckPlayer& player) {
    if (ammo == 0) {
        player.drop_collectable();
        return;
    }
    for (int i = 0; i < 6; ++i) {
        add_bullet(player);
    }
}

SniperG::SniperG(Gun& gun, BulletManager* bullets): GunEntity(gun, bullets) {
    ammo = 3;
    inaccuracy = 0;
    it_to_reload = TICKS / 2;
    it_reloading = it_to_reload;
    range = 64 * BLOCK_SIZE;
}

// void SniperG::start_shooting() {}
// void SniperG::stop_shooting() {}
void SniperG::update_bullets(DuckPlayer& player) {
    if (ammo == 0) {
        player.drop_collectable();
        return;
    }
    add_bullet(player);
}

HelmetG::HelmetG(Gun& gun, BulletManager* bullets): GunEntity(gun, bullets) { ammo = 1; }

// void HelmetG::start_shooting() {}
// void HelmetG::stop_shooting() {}
void HelmetG::update_bullets(DuckPlayer& player) {
    if (trigger_pulled && ammo == 1) {
        player.equip_helmet();
    }
}

ArmorG::ArmorG(Gun& gun, BulletManager* bullets): GunEntity(gun, bullets) { ammo = 1; }

// void ArmorG::start_shooting() {}
// void ArmorG::stop_shooting() {}
void ArmorG::update_bullets(DuckPlayer& player) {
    if (trigger_pulled && ammo == 1) {
        player.equip_armor();
    }
}

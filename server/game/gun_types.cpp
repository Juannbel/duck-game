#include "gun_types.h"

GrenadeG::GrenadeG(Gun& gun) : GunEntity(gun) {
    ammo = 2;
}

void GrenadeG::start_shooting() {}
void GrenadeG::stop_shooting() {}

BananaG::BananaG(Gun& gun) : GunEntity(gun) {
    ammo = 2;
}

void BananaG::start_shooting() {}
void BananaG::stop_shooting() {}

PewPewLaserG::PewPewLaserG(Gun& gun) : GunEntity(gun) {
    ammo = 12*3;
}

void PewPewLaserG::start_shooting() {}
void PewPewLaserG::stop_shooting() {}

LaserRifleG::LaserRifleG(Gun& gun) : GunEntity(gun) {
    ammo = 10;
}

void LaserRifleG::start_shooting() {}
void LaserRifleG::stop_shooting() {}

Ak47G::Ak47G(Gun& gun) : GunEntity(gun) {
    ammo = 30;
}

void Ak47G::start_shooting() {}
void Ak47G::stop_shooting() {}

DuelingPistolG::DuelingPistolG(Gun& gun) : GunEntity(gun) {
    ammo = 1;
}

void DuelingPistolG::start_shooting() {}
void DuelingPistolG::stop_shooting() {}

CowboyPistolG::CowboyPistolG(Gun& gun) : GunEntity(gun) {
    ammo = 6;
}

void CowboyPistolG::start_shooting() {}
void CowboyPistolG::stop_shooting() {}

MagnumG::MagnumG(Gun& gun) : GunEntity(gun) {
    ammo = 6;
}

void MagnumG::start_shooting() {}
void MagnumG::stop_shooting() {}

ShootgunG::ShootgunG(Gun& gun) : GunEntity(gun) {
    ammo = 2;
}

void ShootgunG::start_shooting() {}
void ShootgunG::stop_shooting() {}

SniperG::SniperG(Gun& gun) : GunEntity(gun) {
    ammo = 3;
}

void SniperG::start_shooting() {}
void SniperG::stop_shooting() {}

HelmetG::HelmetG(Gun& gun) : GunEntity(gun) {
    ammo = 1;
}

void HelmetG::start_shooting() {}
void HelmetG::stop_shooting() {}

ArmorG::ArmorG(Gun& gun) : GunEntity(gun) {
    ammo = 1;
}

void ArmorG::start_shooting() {}
void ArmorG::stop_shooting() {}

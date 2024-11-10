#ifndef GUN_ENTITY_TYPES_H
#define GUN_ENTITY_TYPES_H

#include "gun_entity.h"

class GrenadeG: public GunEntity {
public:
    GrenadeG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    // void start_shooting() override;
    void stop_shooting() override;
    void update_bullets(DuckPlayer& player) override;
};

class BananaG: public GunEntity {
public:
    BananaG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    // void start_shooting() override;
    void stop_shooting() override;
    void update_bullets(DuckPlayer& player) override;
};

class PewPewLaserG: public GunEntity {
public:
    PewPewLaserG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    // void start_shooting() override;
    // void stop_shooting() override;
    void update_bullets(DuckPlayer& player) override;
};

class LaserRifleG: public GunEntity {
public:
    LaserRifleG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    // void start_shooting() override;
    // void stop_shooting() override;
    void update_bullets(DuckPlayer& player) override;
};

class Ak47G: public GunEntity {
public:
    Ak47G(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    // void start_shooting() override;
    // void stop_shooting() override;
    void update_bullets(DuckPlayer& player) override;
};

class DuelingPistolG: public GunEntity {
public:
    DuelingPistolG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    // void start_shooting() override;
    // void stop_shooting() override;
    void update_bullets(DuckPlayer& player) override;
};

class CowboyPistolG: public GunEntity {
public:
    CowboyPistolG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    // void start_shooting() override;
    // void stop_shooting() override;
    void update_bullets(DuckPlayer& player) override;
};

class MagnumG: public GunEntity {
public:
    MagnumG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    // void start_shooting() override;
    // void stop_shooting() override;
    void update_bullets(DuckPlayer& player) override;
};

class ShootgunG: public GunEntity {
public:
    ShootgunG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    // void start_shooting() override;
    // void stop_shooting() override;
    void update_bullets(DuckPlayer& player) override;
};

class SniperG: public GunEntity {
public:
    SniperG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    // void start_shooting() override;
    // void stop_shooting() override;
    void update_bullets(DuckPlayer& player) override;
};

class HelmetG: public GunEntity {
public:
    HelmetG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    // void start_shooting() override;
    // void stop_shooting() override;
    void update_bullets(DuckPlayer& player) override;
};

class ArmorG: public GunEntity {
public:
    ArmorG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    // void start_shooting() override;
    // void stop_shooting() override;
    void update_bullets(DuckPlayer& player) override;
};

#endif

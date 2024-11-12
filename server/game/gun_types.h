#ifndef GUN_ENTITY_TYPES_H
#define GUN_ENTITY_TYPES_H

#include "gun_entity.h"
#include "ticks.h"

#define GRENADE_HITBOX_WIDTH 16
#define GRENADE_HITBOX_HEIGHT 16

class GrenadeG: public GunEntity {
public:
    GrenadeG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions, bool explode);
    void start_shooting() override;
    void stop_shooting() override;
    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
    void update_status() override;
    void explode_grenade() override;
};

#define BANANA_HITBOX_WIDTH 16
#define BANANA_HITBOX_HEIGHT 16

class BananaG: public GunEntity {
public:
    BananaG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    void throw_gun(bool facing_right) override;
    void start_shooting() override;
    void stop_shooting() override;
    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
};

#define PEWPEW_LASER_HITBOX_WIDTH 16
#define PEWPEW_LASER_HITBOX_HEIGHT 16

class PewPewLaserG: public GunEntity {
public:
    PewPewLaserG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    // void start_shooting() override;
    // void stop_shooting() override;
    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
};

#define LASER_RIFLE_HITBOX_WIDTH 16
#define LASER_RIFLE_HITBOX_HEIGHT 16

class LaserRifleG: public GunEntity {
public:
    LaserRifleG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    // void start_shooting() override;
    // void stop_shooting() override;
    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
};

#define AK47_HITBOX_WIDTH 16
#define AK47_HITBOX_HEIGHT 16

class Ak47G: public GunEntity {
public:
    Ak47G(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    // void start_shooting() override;
    // void stop_shooting() override;
    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
};

#define DUELING_PISTOL_HITBOX_WIDTH 16
#define DUELING_PISTOL_HITBOX_HEIGHT 16

class DuelingPistolG: public GunEntity {
public:
    DuelingPistolG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    // void start_shooting() override;
    // void stop_shooting() override;
    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
};

#define COWBOY_PISTOL_HITBOX_WIDTH 16
#define COWBOY_PISTOL_HITBOX_HEIGHT 16

class CowboyPistolG: public GunEntity {
public:
    CowboyPistolG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    // void start_shooting() override;
    // void stop_shooting() override;
    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
};

#define MAGNUM_HITBOX_WIDTH 16
#define MAGNUM_HITBOX_HEIGHT 16

class MagnumG: public GunEntity {
public:
    MagnumG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    // void start_shooting() override;
    // void stop_shooting() override;
    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
};

#define SHOTGUN_HITBOX_WIDTH 16
#define SHOTGUN_HITBOX_HEIGHT 16

class ShootgunG: public GunEntity {
public:
    ShootgunG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    // void start_shooting() override;
    // void stop_shooting() override;
    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
};

#define SNIPER_HITBOX_WIDTH 16
#define SNIPER_HITBOX_HEIGHT 16

class SniperG: public GunEntity {
public:
    SniperG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    // void start_shooting() override;
    // void stop_shooting() override;
    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
};


#define HELMET_HITBOX_WIDTH 16
#define HELMET_HITBOX_HEIGHT 16

class HelmetG: public GunEntity {
public:
    HelmetG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    // void start_shooting() override;
    // void stop_shooting() override;
    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
};

#define ARMOR_HITBOX_WIDTH 16
#define ARMOR_HITBOX_HEIGHT 16

class ArmorG: public GunEntity {
public:
    ArmorG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    // void start_shooting() override;
    // void stop_shooting() override;
    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
};

#endif

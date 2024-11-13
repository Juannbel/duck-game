#ifndef GUN_ENTITY_TYPES_H
#define GUN_ENTITY_TYPES_H

#include "guns/ak47.h"
#include "guns/armor.h"
#include "guns/banana.h"
#include "guns/cowboy_pistol.h"
#include "guns/dueling_pistol.h"
#include "guns/grenade.h"
#include "guns/helmet.h"
#include "guns/laser_rifle.h"
#include "guns/magnum.h"
#include "guns/pewpew_laser.h"
#include "guns/shotgun.h"
#include "guns/sniper.h"

#include "gun_entity.h"

//#include "ticks.h"
//
//#define GRENADE_HITBOX_WIDTH 12
//#define GRENADE_HITBOX_HEIGHT 12
//
// class GrenadeG: public GunEntity {
// public:
//    GrenadeG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions, bool explode);
//    void start_shooting() override;
//    void stop_shooting() override;
//    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
//    void update_status() override;
//    void explode_grenade() override;
//};
//
//#define BANANA_HITBOX_WIDTH 12
//#define BANANA_HITBOX_HEIGHT 16
//
// class BananaG: public GunEntity {
// public:
//    BananaG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
//    void throw_gun(bool facing_right) override;
//    void start_shooting() override;
//    void stop_shooting() override;
//    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
//};
//
//#define PEWPEW_LASER_HITBOX_WIDTH 28
//#define PEWPEW_LASER_HITBOX_HEIGHT 13
//
// class PewPewLaserG: public GunEntity {
// public:
//    PewPewLaserG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
//    // void start_shooting() override;
//    // void stop_shooting() override;
//    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
//};
//
//#define LASER_RIFLE_HITBOX_WIDTH 25
//#define LASER_RIFLE_HITBOX_HEIGHT 15
//
// class LaserRifleG: public GunEntity {
// public:
//    LaserRifleG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
//    // void start_shooting() override;
//    // void stop_shooting() override;
//    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
//};
//
//#define AK47_HITBOX_WIDTH 32
//#define AK47_HITBOX_HEIGHT 14
//
// class Ak47G: public GunEntity {
// public:
//    Ak47G(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
//    // void start_shooting() override;
//    // void stop_shooting() override;
//    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
//};
//
//#define DUELING_PISTOL_HITBOX_WIDTH 15
//#define DUELING_PISTOL_HITBOX_HEIGHT 12
//
// class DuelingPistolG: public GunEntity {
// public:
//    DuelingPistolG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
//    // void start_shooting() override;
//    // void stop_shooting() override;
//    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
//};
//
//#define COWBOY_PISTOL_HITBOX_WIDTH 26
//#define COWBOY_PISTOL_HITBOX_HEIGHT 14
//
// class CowboyPistolG: public GunEntity {
// public:
//    CowboyPistolG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
//    // void start_shooting() override;
//    // void stop_shooting() override;
//    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
//};
//
//#define MAGNUM_HITBOX_WIDTH 24
//#define MAGNUM_HITBOX_HEIGHT 14
//
// class MagnumG: public GunEntity {
// public:
//    MagnumG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
//    // void start_shooting() override;
//    // void stop_shooting() override;
//    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
//};
//
//#define SHOTGUN_HITBOX_WIDTH 30
//#define SHOTGUN_HITBOX_HEIGHT 13
//
// class ShootgunG: public GunEntity {
// public:
//    ShootgunG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
//    // void start_shooting() override;
//    // void stop_shooting() override;
//    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
//};
//
//#define SNIPER_HITBOX_WIDTH 32
//#define SNIPER_HITBOX_HEIGHT 12
//
// class SniperG: public GunEntity {
// public:
//    SniperG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
//    // void start_shooting() override;
//    // void stop_shooting() override;
//    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
//};
//
//
//#define HELMET_HITBOX_WIDTH 15
//#define HELMET_HITBOX_HEIGHT 12
//
// class HelmetG: public GunEntity {
// public:
//    HelmetG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
//    // void start_shooting() override;
//    // void stop_shooting() override;
//    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
//};
//
//#define ARMOR_HITBOX_WIDTH 15
//#define ARMOR_HITBOX_HEIGHT 12
//
// class ArmorG: public GunEntity {
// public:
//    ArmorG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
//    // void start_shooting() override;
//    // void stop_shooting() override;
//    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
//};
//
#endif

#ifndef GUN_ENTITY_TYPES_H
#define GUN_ENTITY_TYPES_H

#include "gun_entity.h"

class GrenadeG : public GunEntity {
    public:
    GrenadeG(Gun& gun, BulletManager* bullets);
    //void start_shooting() override;
    //void stop_shooting() override;
    void update_bullets(const Duck& status) override;
};

class BananaG : public GunEntity {
    public:
    BananaG(Gun& gun, BulletManager* bullets);
    //void start_shooting() override;
    //void stop_shooting() override;
    void update_bullets(const Duck& status) override;
};

class PewPewLaserG : public GunEntity {
    public:
    PewPewLaserG(Gun& gun, BulletManager* bullets);
    //void start_shooting() override;
    //void stop_shooting() override;
    void update_bullets(const Duck& status) override;
};

class LaserRifleG : public GunEntity {
    public:
    LaserRifleG(Gun& gun, BulletManager* bullets);
    //void start_shooting() override;
    //void stop_shooting() override;
    void update_bullets(const Duck& status) override;
};

class Ak47G : public GunEntity {
    public:
    Ak47G(Gun& gun, BulletManager* bullets);
    //void start_shooting() override;
    //void stop_shooting() override;
    void update_bullets(const Duck& status) override;
};

class DuelingPistolG : public GunEntity {
    public:
    DuelingPistolG(Gun& gun, BulletManager* bullets);
    //void start_shooting() override;
    //void stop_shooting() override;
    void update_bullets(const Duck& status) override;
};

class CowboyPistolG : public GunEntity {
    public:
    CowboyPistolG(Gun& gun, BulletManager* bullets);
    //void start_shooting() override;
    //void stop_shooting() override;
    void update_bullets(const Duck& status) override;
};

class MagnumG : public GunEntity {
    public:
    MagnumG(Gun& gun, BulletManager* bullets);
    //void start_shooting() override;
    //void stop_shooting() override;
    void update_bullets(const Duck& status) override;
};

class ShootgunG : public GunEntity {
    public:
    ShootgunG(Gun& gun, BulletManager* bullets);
    //void start_shooting() override;
    //void stop_shooting() override;
    void update_bullets(const Duck& status) override;
};

class SniperG : public GunEntity {
    public:
    SniperG(Gun& gun, BulletManager* bullets);
    //void start_shooting() override;
    //void stop_shooting() override;
    void update_bullets(const Duck& status) override;
};

class HelmetG : public GunEntity {
    public:
    HelmetG(Gun& gun, BulletManager* bullets);
    //void start_shooting() override;
    //void stop_shooting() override;
    void update_bullets(const Duck& status) override;
};

class ArmorG : public GunEntity {
    public:
    ArmorG(Gun& gun, BulletManager* bullets);
    //void start_shooting() override;
    //void stop_shooting() override;
    void update_bullets(const Duck& status) override;
};

#endif

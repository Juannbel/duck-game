#ifndef GUN_ENTITY_TYPES_H
#define GUN_ENTITY_TYPES_H

#include "gun_entity.h"

class GrenadeG : public GunEntity {
    public:
    GrenadeG(Gun& gun);
    void start_shooting() override;
    void stop_shooting() override;
};

class BananaG : public GunEntity {
    public:
    BananaG(Gun& gun);
    void start_shooting() override;
    void stop_shooting() override;
};

class PewPewLaserG : public GunEntity {
    public:
    PewPewLaserG(Gun& gun);
    void start_shooting() override;
    void stop_shooting() override;
};

class LaserRifleG : public GunEntity {
    public:
    LaserRifleG(Gun& gun);
    void start_shooting() override;
    void stop_shooting() override;
};

class Ak47G : public GunEntity {
    public:
    Ak47G(Gun& gun);
    void start_shooting() override;
    void stop_shooting() override;
};

class DuelingPistolG : public GunEntity {
    public:
    DuelingPistolG(Gun& gun);
    void start_shooting() override;
    void stop_shooting() override;
};

class CowboyPistolG : public GunEntity {
    public:
    CowboyPistolG(Gun& gun);
    void start_shooting() override;
    void stop_shooting() override;
};

class MagnumG : public GunEntity {
    public:
    MagnumG(Gun& gun);
    void start_shooting() override;
    void stop_shooting() override;
};

class ShootgunG : public GunEntity {
    public:
    ShootgunG(Gun& gun);
    void start_shooting() override;
    void stop_shooting() override;
};

class SniperG : public GunEntity {
    public:
    SniperG(Gun& gun);
    void start_shooting() override;
    void stop_shooting() override;
};

class HelmetG : public GunEntity {
    public:
    HelmetG(Gun& gun);
    void start_shooting() override;
    void stop_shooting() override;
};

class ArmorG : public GunEntity {
    public:
    ArmorG(Gun& gun);
    void start_shooting() override;
    void stop_shooting() override;
};

#endif

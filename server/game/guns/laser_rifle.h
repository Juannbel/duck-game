#ifndef LASER_RIFLE_H
#define LASER_RIFLE_H

#include "server/game/gun_entity.h"

#define LASER_RIFLE_HITBOX_WIDTH 25
#define LASER_RIFLE_HITBOX_HEIGHT 15

class LaserRifleG: public GunEntity {
public:
    LaserRifleG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
    void stop_shooting() override;
};

#endif

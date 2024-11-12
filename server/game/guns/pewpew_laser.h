#ifndef PEWPEW_LASER_H
#define PEWPEW_LASER_H

#include "server/game/gun_entity.h"

#define PEWPEW_LASER_HITBOX_WIDTH 28
#define PEWPEW_LASER_HITBOX_HEIGHT 13

class PewPewLaserG: public GunEntity {
public:
    PewPewLaserG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
};

#endif

#ifndef PEWPEW_LASER_H
#define PEWPEW_LASER_H

#include "server/game/gun_entity.h"

class PewPewLaserG: public GunEntity {
public:
    PewPewLaserG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    // void start_shooting() override;
    // void stop_shooting() override;
    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
};

#endif

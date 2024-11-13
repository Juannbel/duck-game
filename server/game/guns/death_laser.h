#ifndef DEATH_LASER_H
#define DEATH_LASER_H

#include "server/game/gun_entity.h"

#define DEATH_LASER_HITBOX_WIDTH 24
#define DEATH_LASER_HITBOX_HEIGHT 15

class DeathLaserG: public GunEntity {
public:
    DeathLaserG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
    void stop_shooting() override;
};

#endif

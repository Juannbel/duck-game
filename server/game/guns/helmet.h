#ifndef HELMET_H
#define HELMET_H

#include "server/game/gun_entity.h"

class HelmetG: public GunEntity {
public:
    HelmetG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    // void start_shooting() override;
    // void stop_shooting() override;
    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
};

#endif

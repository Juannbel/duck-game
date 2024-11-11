#ifndef SNIPER_H
#define SNIPER_H

#include "server/game/gun_entity.h"

class SniperG: public GunEntity {
public:
    SniperG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    // void start_shooting() override;
    // void stop_shooting() override;
    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
};

#endif

#ifndef AK47_H
#define AK47_H

#include "server/game/gun_entity.h"

class Ak47G: public GunEntity {
public:
    Ak47G(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    // void start_shooting() override;
    // void stop_shooting() override;
    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
};

#endif

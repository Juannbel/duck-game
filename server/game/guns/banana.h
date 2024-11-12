#ifndef BANANA_H
#define BANANA_H

#include "server/game/gun_entity.h"

class BananaG: public GunEntity {
public:
    BananaG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    void throw(bool facing_right) override;
    void stop_shooting() override;
    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
};

#endif

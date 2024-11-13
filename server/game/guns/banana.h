#ifndef BANANA_H
#define BANANA_H

#include "server/game/gun_entity.h"

#define BANANA_HITBOX_WIDTH 12
#define BANANA_HITBOX_HEIGHT 16

class BananaG: public GunEntity {
public:
    BananaG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    void throw_gun(bool facing_right) override;
    void start_shooting() override;
    void stop_shooting() override;
    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
};

#endif

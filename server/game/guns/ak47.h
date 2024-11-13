#ifndef AK47_H
#define AK47_H

#include "server/game/gun_entity.h"

#define AK47_HITBOX_WIDTH 32
#define AK47_HITBOX_HEIGHT 14

class Ak47G: public GunEntity {
public:
    Ak47G(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
    void stop_shooting() override;
};

#endif

#ifndef HELMET_H
#define HELMET_H

#include "server/game/gun_entity.h"

#define HELMET_HITBOX_WIDTH 15
#define HELMET_HITBOX_HEIGHT 12

class HelmetG: public GunEntity {
public:
    HelmetG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
};

#endif

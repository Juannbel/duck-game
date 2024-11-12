#ifndef MAGNUM_H
#define MAGNUM_H

#include "server/game/gun_entity.h"

#define MAGNUM_HITBOX_WIDTH 24
#define MAGNUM_HITBOX_HEIGHT 14

class MagnumG: public GunEntity {
public:
    MagnumG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
};

#endif

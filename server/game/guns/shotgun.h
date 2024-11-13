#ifndef SHOTGUN_H
#define SHOTGUN_H

#include "server/game/gun_entity.h"

#define SHOTGUN_HITBOX_WIDTH 30
#define SHOTGUN_HITBOX_HEIGHT 13

class ShootgunG: public GunEntity {
public:
    ShootgunG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
};

#endif

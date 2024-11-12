#ifndef SNIPER_H
#define SNIPER_H

#include "server/game/gun_entity.h"

#define SNIPER_HITBOX_WIDTH 32
#define SNIPER_HITBOX_HEIGHT 12


class SniperG: public GunEntity {
public:
    SniperG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
};

#endif

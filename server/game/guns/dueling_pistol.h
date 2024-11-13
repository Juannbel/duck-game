#ifndef DUELING_PISTOL_H
#define DUELING_PISTOL_H

#include "server/game/gun_entity.h"

#define DUELING_PISTOL_HITBOX_WIDTH 15
#define DUELING_PISTOL_HITBOX_HEIGHT 12

class DuelingPistolG: public GunEntity {
public:
    DuelingPistolG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
};

#endif

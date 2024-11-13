#ifndef COWBOY_PISTOL_H
#define COWBOY_PISTOL_H

#include "server/game/gun_entity.h"

#define COWBOY_PISTOL_HITBOX_WIDTH 26
#define COWBOY_PISTOL_HITBOX_HEIGHT 14

class CowboyPistolG: public GunEntity {
public:
    CowboyPistolG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
};

#endif

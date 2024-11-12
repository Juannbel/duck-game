#ifndef ARMOR_H
#define ARMOR_H

#include "server/game/gun_entity.h"

#define ARMOR_HITBOX_WIDTH 15
#define ARMOR_HITBOX_HEIGHT 12

class ArmorG: public GunEntity {
public:
    ArmorG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    // void start_shooting() override;
    // void stop_shooting() override;
    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
};

#endif

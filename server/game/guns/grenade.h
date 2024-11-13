#ifndef GRENADE_H
#define GRENADE_H

#include "server/game/gun_entity.h"

#define GRENADE_HITBOX_WIDTH 12
#define GRENADE_HITBOX_HEIGHT 12

class GrenadeG: public GunEntity {
public:
    GrenadeG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions, bool explode);
    void throw_gun(bool facing_right) override;
    void start_shooting() override;
    void stop_shooting() override;
    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
    void update_status() override;
    void explode_grenade() override;
};

#endif

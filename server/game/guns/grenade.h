#ifndef GRENADE_H
#define GRENADE_H

#include "server/game/gun_entity.h"

class GrenadeG: public GunEntity {
public:
    GrenadeG(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);
    // void start_shooting() override;
    void stop_shooting() override;
    bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) override;
    void update_status() override;
    void explode_grenade() override;
};

#endif

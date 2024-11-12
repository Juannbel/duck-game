#ifndef BOX_H
#define BOX_H

#include <cstdint>
#include "common/snapshot.h"
#include "common/shared_constants.h"
#include "server/game/collisions.h"

const float MAX_BOX_HP = 100;

class BoxEntity {
    private:
    Box info;
    Rectangle hitbox;
    float health;
    CollisionChecks& collisions;
    public:
    BoxEntity(float x, float y, uint32_t id, CollisionChecks& collisions);
    bool get_hit(const Rectangle& b_rect, uint8_t damage);
    bool destroyed();
    Coordenades& get_coords();
    Box get_info();
};

#endif

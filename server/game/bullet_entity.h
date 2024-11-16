#ifndef BULLET_ENTITY_H
#define BULLET_ENTITY_H

#include <cstdint>
#include <unordered_map>

#include "common/snapshot.h"
#include "server/game/boxes.h"
#include "server/game/collisions.h"

class DuckPlayer;

class BulletEntity {
private:
    Bullet status;
    Rectangle hitbox;
    float speed;
    uint8_t damage;
    uint16_t range;
    bool is_alive;
    CollisionChecks& collisions;
    std::unordered_map<uint8_t, DuckPlayer>& ducks;
    std::unordered_map<uint32_t, BoxEntity>& boxes;
    bool check_collision_with_boxes_ducks();
    void check_collision_and_change_angle(float new_x, float new_y);
    void update_angle(Collision collision);
    void move_banana();

public:
    BulletEntity(const Rectangle& info, CollisionChecks& collision_ckecker,
                 std::unordered_map<uint8_t, DuckPlayer>& ducks,
                 std::unordered_map<uint32_t, BoxEntity>& boxes, int16_t angle, GunType type,
                 uint32_t id);
    void update_status();
    Bullet& get_status();
    bool still_alive();
};

#endif

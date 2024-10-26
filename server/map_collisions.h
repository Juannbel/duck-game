#ifndef MAP_COLLISION_H
#define MAP_COLLISION_H

#include "common/snapshot.h"
#include "gun_entity.h"

#include <map>
#include <vector>
#include <cstdint>

struct Coordenades {
    float x;
    float y;
};

struct Rectangle {
    Coordenades coords;
    float width;
    float height;
};

struct Collision {
    Coordenades last_valid_position;
    bool vertical_collision;
    bool horizontal_collision;
};

class MapCollisions{
private:
    std::map<uint16_t, std::vector<Rectangle>> blocks;
    std::map<uint32_t, GunEntity> guns;
    uint32_t collectable_id;

public:
    MapCollisions();
    
    void add_block(float x, float y);
    uint32_t get_and_inc_collectable_id();
    void add_gun(Gun& gun);
    void add_gun(GunEntity& gun);
    void drop_gun(GunEntity &&gun, const Rectangle& duck_hitbox);
    void move_guns_falling();
    GunEntity pickup(const Rectangle &duck);

    struct Collision check_near_blocks_collision(struct Rectangle& entity, float new_x,
                                                 float new_y);
    struct Collision rectangles_collision(const struct Rectangle& r1, const struct Rectangle& r2);

    void add_guns_to_snapshot(Snapshot& snapshot);
};

#endif

#ifndef MAP_COLLISION_H
#define MAP_COLLISION_H

#include "common/snapshot.h"
#include "gun_entity.h"

#include <map>
#include <vector>
#include <cstdint>

struct Rectangle {
    int16_t x;
    int16_t y;
    int16_t width;
    int16_t height;
};

struct Collision {
    Rectangle last_valid_position;
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
    
    void add_block(int16_t x, int16_t y);
    uint32_t get_and_inc_collectable_id();
    void add_gun(Gun& gun);
    void add_gun(GunEntity& gun);

    GunEntity pickup(const Rectangle &duck);

    struct Collision check_near_blocks_collision(struct Rectangle& entity, int16_t new_x,
                                                 int16_t new_y);
    struct Collision rectangles_collision(const struct Rectangle& r1, const struct Rectangle& r2);

    void add_guns_to_snapshot(Snapshot& snapshot);
};

#endif

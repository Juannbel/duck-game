#ifndef MAP_COLLISION_H
#define MAP_COLLISION_H

#include "common/snapshot.h"

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
    std::map<uint32_t, Gun> guns;

public:
    MapCollisions();
    
    void add_block(int16_t x, int16_t y);
    void add_gun(Gun& gun);

    GunType pickup(const Rectangle &duck);

    struct Collision check_near_blocks_collision(struct Rectangle& entity, int16_t new_x,
                                                 int16_t new_y);
    struct Collision rectangles_collision(const struct Rectangle& r1, const struct Rectangle& r2);

    void add_guns_to_snapshot(Snapshot& snapshot);
};

#endif

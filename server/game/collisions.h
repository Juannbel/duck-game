#ifndef COLLISIONS_H
#define COLLISIONS_H

#include <cstdint>
#include <map>
#include <utility>

#include "common/map_dto.h"

struct Coordenades {
    float x;
    float y;
};

struct Rectangle {
    Coordenades coords;
    float width;
    float height;
};

struct BlockInfo {
    Rectangle hitbox;
    bool solid;
};

struct Collision {
    Coordenades last_valid_position;
    bool vertical_collision;
    bool horizontal_collision;
};

class CollisionChecks {
private:
    std::map<std::pair<int16_t, int16_t>, BlockInfo> blocks;
    void add_block(float x, float y, bool half, bool solid);
    Collision check_collisions_with_block(BlockInfo& block, Rectangle& final_rec, Rectangle& entity,
                                                   float new_y);


public:
    CollisionChecks(): blocks() {}
    void load_map(const MapDto& map_dto);

    bool out_of_map(float x, float y);

    struct Collision check_near_blocks_collision(struct Rectangle& entity, float new_x,
                                                 float new_y);
    struct Collision rectangles_collision(const struct Rectangle& r1, const struct Rectangle& r2);
};

#endif

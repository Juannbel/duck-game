#ifndef COLLISIONS_H
#define COLLISIONS_H

#include <cstdint>
#include <map>
#include <vector>

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
    std::map<uint16_t, std::vector<BlockInfo>> blocks;
    void add_block(float x, float y, bool half, bool solid);
    Collision check_collisions_in_row(std::vector<BlockInfo>& block_columns, Rectangle& final_rec,
                                      Rectangle& entity, float new_y);


public:
    CollisionChecks(): blocks() {}
    void load_map(MapDto&);

    bool out_of_map(float x, float y);

    struct Collision check_near_blocks_collision(struct Rectangle& entity, float new_x,
                                                 float new_y);
    struct Collision rectangles_collision(const struct Rectangle& r1, const struct Rectangle& r2);
};

#endif

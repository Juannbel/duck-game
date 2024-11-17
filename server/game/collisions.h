#ifndef COLLISIONS_H
#define COLLISIONS_H

#include <cstdint>
#include <map>
#include <utility>

#include "common/map_dto.h"

struct Coordenades {
    // cppcheck-suppress unusedStructMember
    float x;
    // cppcheck-suppress unusedStructMember
    float y;
};

struct Rectangle {
    Coordenades coords;
    // cppcheck-suppress unusedStructMember
    float width;
    // cppcheck-suppress unusedStructMember
    float height;
};

struct BlockInfo {
    Rectangle hitbox;
    // cppcheck-suppress unusedStructMember
    bool solid;
};

struct Collision {
    Coordenades last_valid_position;
    // cppcheck-suppress unusedStructMember
    bool vertical_collision;
    // cppcheck-suppress unusedStructMember
    bool horizontal_collision;
};

class CollisionChecks {
private:
    std::map<std::pair<int16_t, int16_t>, BlockInfo> blocks;
    void add_block(float x, float y, bool half, bool solid);
    Collision check_collisions_with_block(const BlockInfo& block, Rectangle& final_rec,
                                          const Rectangle& entity, float new_y);


public:
    CollisionChecks(): blocks() {}
    void load_map(const MapDto& map_dto);

    bool out_of_map(float x, float y);

    Collision check_near_blocks_collision(const Rectangle& entity, float new_x, float new_y);
    Collision rectangles_collision(const Rectangle& r1, const Rectangle& r2);
};

#endif

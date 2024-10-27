#ifndef COLLISIONS_H
#define COLLISIONS_H

#include "common/map_dto.h"

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

class CollisionChecks {
    private:
    std::map<uint16_t, std::vector<Rectangle>> blocks;
    void add_block(float x, float y);
    public:
    CollisionChecks() {}
    void load_map(Map&);

    bool out_of_map(float x, float y);

    struct Collision check_near_blocks_collision(struct Rectangle& entity, float new_x,
                                                 float new_y);
    struct Collision rectangles_collision(const struct Rectangle& r1, const struct Rectangle& r2);

};

#endif

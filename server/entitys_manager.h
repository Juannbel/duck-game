#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <cstdint>
#include <map>
#include <vector>

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

class EntityManager {
private:
    std::map<uint16_t, std::vector<Rectangle>> blocks;

public:
    EntityManager();
    void add_block(int16_t x, int16_t y);
    struct Collision check_near_blocks_collision(struct Rectangle& entity, int16_t new_x,
                                                 int16_t new_y);
    struct Collision rectangles_collision(const struct Rectangle& r1, const struct Rectangle& r2);

    ~EntityManager();
};


#endif

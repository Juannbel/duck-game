#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <cstdint>
#include <map>
#include <vector>
#include "common/map_dto.h"
#include "common/snapshot.h"

struct Rectangle {
    int16_t x;
    int16_t y;
    int16_t width;
    int16_t height;
};

struct GunPosition {
    int16_t x;
    int16_t y;
    uint32_t id;
};

struct Collision {
    Rectangle last_valid_position;
    bool vertical_collision;
    bool horizontal_collision;
};

class EntityManager {
private:
    std::map<uint16_t, std::vector<Rectangle>> blocks;
    std::map<uint32_t, Gun> guns; // Probablemente sea collectables despues
    std::map<uint32_t, Bullet> bullets;

public:
    EntityManager(Map& map_dto);
    void add_block(int16_t x, int16_t y);
    void add_gun(Gun& gun);
    GunType pickup(const Rectangle& duck);
    struct Collision check_near_blocks_collision(struct Rectangle& entity, int16_t new_x,
                                                 int16_t new_y);
    struct Collision rectangles_collision(const struct Rectangle& r1, const struct Rectangle& r2);

    void add_guns_bullets_to_snapshot(Snapshot &snapshot);

    ~EntityManager();
};


#endif

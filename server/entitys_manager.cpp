#include "entitys_manager.h"

#include <iostream>

#include "common/shared_constants.h"

const int16_t NEAR_CELLS = 3;

EntityManager::EntityManager(): blocks() {}

void EntityManager::add_block(int16_t x, int16_t y) {
    Rectangle rectangle = {x, y, BLOCK_SIZE, BLOCK_SIZE};
    blocks[y / BLOCK_SIZE].push_back(rectangle);
}

struct Collision EntityManager::check_near_blocks_collision(struct Rectangle& entity, int16_t new_x,
                                                            int16_t new_y) {
    int16_t row_index = entity.y / BLOCK_SIZE;
    int16_t i = (row_index < NEAR_CELLS) ? 0 : row_index - NEAR_CELLS;
    int16_t end_i = (row_index + NEAR_CELLS > MAP_HEIGHT_BLOCKS) ? MAP_HEIGHT_BLOCKS :
                                                                   row_index + NEAR_CELLS;

    struct Rectangle final_rec = {new_x, new_y, entity.width, entity.height};
    struct Collision collision;
    collision.horizontal_collision = false;
    collision.vertical_collision = false;
    for (; i < end_i; ++i) {
        if (blocks.find(i) == blocks.end()) {
            continue;
        }
        std::vector<Rectangle>& block_columns = blocks[i];
        for (auto& block: block_columns) {
            struct Collision aux_collision = rectangles_collision(final_rec, block);
            if (aux_collision.horizontal_collision) {
                final_rec.x = entity.x;
                collision.horizontal_collision = true;
                bool vertical_collision = rectangles_collision(final_rec, block).vertical_collision;
                if (vertical_collision) {
                    if (new_y > entity.y && new_y + entity.height > block.y && entity.y < block.y) {
                        final_rec.y = block.y - entity.height;
                    } else if (new_y < block.y + block.height && entity.y > block.y) {
                        final_rec.y = entity.y;
                    }
                    collision.vertical_collision = true;
                }
            } else if (aux_collision.vertical_collision) {
                collision.vertical_collision = true;
            }
        }
    }
    collision.last_valid_position = final_rec;
    return collision;
}

struct Collision EntityManager::rectangles_collision(const struct Rectangle& r1,
                                                     const struct Rectangle& r2) {
    struct Collision collision;
    collision.horizontal_collision = false;
    collision.vertical_collision = false;
    if ((r1.y <= r2.y && r1.y + r1.height >= r2.y) ||
        (r1.y <= r2.y + r2.height && r1.y + r1.height >= r2.y + r2.height)) {
        if ((r1.x >= r2.x && r1.x <= r2.x + r2.width) ||
            (r1.x + r1.width >= r2.x && r1.x + r1.width <= r2.x + r2.width)) {
            collision.vertical_collision = true;
            collision.horizontal_collision = true;
            if (r1.y + r1.height == r2.y || r1.y == r2.y + r2.height) {
                collision.horizontal_collision = false;
            }
            return collision;
        }
    }
    return collision;
}


EntityManager::~EntityManager() {}

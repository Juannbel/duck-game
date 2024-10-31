#include "collisions.h"
#include <iostream>

#include "common/map_dto.h"
#include "common/shared_constants.h"

const int16_t NEAR_CELLS = 3;
const float MAP_EDGE = 50;

void CollisionChecks::add_block(float x, float y, bool half, bool solid) {
    float heigh = half ? static_cast<float>(BLOCK_SIZE)/2 : BLOCK_SIZE;
    Rectangle rectangle = {{x, y}, BLOCK_SIZE, heigh};
    blocks[y / BLOCK_SIZE].push_back({rectangle, solid});
}

void CollisionChecks::load_map(Map& map_dto) {
    blocks.clear();
    for (int16_t i = 0; i < MAP_HEIGHT_BLOCKS; ++i) {
        for (int16_t j = 0; j < MAP_WIDTH_BLOCKS; ++j) {
            if (map_dto.blocks[i][j].type != BlockType::Empty) {
                bool half = map_dto.blocks[i][j].type == HalfFloor;
                bool solid = map_dto.blocks[i][j].solid;
                add_block(j * BLOCK_SIZE, i * BLOCK_SIZE, half, solid);
            }
        }
    }
}

bool CollisionChecks::out_of_map(float x, float y) {
    return x < -MAP_EDGE || x > MAP_WIDTH_PIXELS + MAP_EDGE || y > MAP_HEIGHT_PIXELS + MAP_EDGE;
}

bool check_collision_with_no_solid(bool vertical_collision, float new_y, Rectangle &entity, Rectangle& block_hb) {
    if (new_y < entity.coords.y && vertical_collision) {
        return true;
    } else if (new_y >= entity.coords.y && vertical_collision 
            && entity.coords.y+entity.height > block_hb.coords.y) {
        return true;
    }
    return false;
}

Collision CollisionChecks::check_collisions_in_row(std::vector<BlockInfo>& block_columns, Rectangle& final_rec, Rectangle& entity,float new_y) {
    struct Collision collision = {final_rec.coords, false, false};
    for (auto& block: block_columns) {
        Rectangle& block_hb = block.hitbox;
        Collision aux_collision = rectangles_collision(final_rec, block_hb);
        if (!block.solid) {
            if (check_collision_with_no_solid(aux_collision.vertical_collision, new_y, entity, block_hb))
                continue;
        }
        if (aux_collision.horizontal_collision) {
            final_rec.coords.x = entity.coords.x;
            collision.horizontal_collision = true;
            bool vertical_collision = rectangles_collision(final_rec, block_hb).vertical_collision;
            if (vertical_collision) {
                Coordenades& entity_c = entity.coords;
                Coordenades& block_c = block_hb.coords;
                if (new_y > entity_c.y && new_y + entity.height > block_c.y &&
                    entity_c.y < block_c.y) {
                    final_rec.coords.y = block_c.y - entity.height;
                } else if (new_y < block_c.y + block_hb.height && entity_c.y > block_c.y) {
                    final_rec.coords.y = block_c.y + block_hb.height;
                }
                collision.vertical_collision = true;
            }
        } else if (aux_collision.vertical_collision) {
            collision.vertical_collision = true;
        }
    }
    collision.last_valid_position = final_rec.coords;
    return collision;
}

struct Collision CollisionChecks::check_near_blocks_collision(struct Rectangle& entity, float new_x,
                                                              float new_y) {
    int16_t row_index = entity.coords.y / BLOCK_SIZE;
    int16_t i = (row_index < NEAR_CELLS) ? 0 : row_index - NEAR_CELLS;
    int16_t end_i = (row_index + NEAR_CELLS > MAP_HEIGHT_BLOCKS) ? MAP_HEIGHT_BLOCKS :
                                                                   row_index + NEAR_CELLS;

    struct Rectangle final_rec = {{new_x, new_y}, entity.width, entity.height};
    struct Collision collision;
    collision.horizontal_collision = false;
    collision.vertical_collision = false;
    for (; i < end_i; ++i) {
        if (blocks.find(i) == blocks.end()) {
            continue;
        }
        std::vector<BlockInfo>& block_columns = blocks[i];
        Collision collision_in_row  = check_collisions_in_row(block_columns, final_rec, entity, new_y);
        collision.horizontal_collision = collision_in_row.horizontal_collision ? collision_in_row.horizontal_collision : collision.horizontal_collision;
        collision.vertical_collision = collision_in_row.vertical_collision ? collision_in_row.vertical_collision : collision.vertical_collision;
    }
    collision.last_valid_position = final_rec.coords;
    return collision;
}

struct Collision CollisionChecks::rectangles_collision(const struct Rectangle& r1,
                                                       const struct Rectangle& r2) {
    struct Collision collision;
    collision.horizontal_collision = false;
    collision.vertical_collision = false;
    const Coordenades& r1_c = r1.coords;
    const Coordenades& r2_c = r2.coords;
    if (r1_c.y <= r2_c.y + r2.height && r1_c.y + r1.height >= r2_c.y) {
        if (r1.coords.x <= r2_c.x + r2.width && r1_c.x + r1.width >= r2_c.x) {
            collision.vertical_collision = true;
            collision.horizontal_collision = true;
            if (r1_c.y + r1.height == r2_c.y || r1_c.y == r2_c.y + r2.height) {
                collision.horizontal_collision = false;
            }
            if (r1_c.y == r2_c.y + r2.height) {
                collision.vertical_collision = false;
            }
            return collision;
        }
    }
    return collision;
}

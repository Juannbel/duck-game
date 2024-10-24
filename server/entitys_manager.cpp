#include "entitys_manager.h"

#include <iostream>

#include "common/map_dto.h"
#include "common/shared_constants.h"

const int16_t NEAR_CELLS = 3;

EntityManager::EntityManager(Map& map_dto) {
    for (int16_t i = 0; i < MAP_HEIGHT_BLOCKS; ++i) {
        for (int16_t j = 0; j < MAP_WIDTH_BLOCKS; ++j) {
            if (map_dto.blocks[i][j] != BlockType::Empty) {
                add_block(j * BLOCK_SIZE, i * BLOCK_SIZE);
            }
        }
    }
}

void EntityManager::add_block(int16_t x, int16_t y) {
    Rectangle rectangle = {x, y, BLOCK_SIZE, BLOCK_SIZE};
    blocks[y / BLOCK_SIZE].push_back(rectangle);
}

void EntityManager::add_gun(Gun& gun) {
    guns[gun.gun_id] = gun;
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

GunType EntityManager::pickup(const Rectangle &duck) {
    Rectangle gun_r = {0, 0, COLLECTABLE_HITBOX_WIDTH, COLLECTABLE_HITBOX_HEIGHT};
    for (auto const& [id, gun] : guns) {
        gun_r.x = gun.x;
        gun_r.y = gun.y;
        Collision collision = rectangles_collision(duck, gun_r);
        if(collision.horizontal_collision && collision.vertical_collision){
            return gun.type;
        }
    }
    return None;
}

struct Collision EntityManager::rectangles_collision(const struct Rectangle& r1,
                                                     const struct Rectangle& r2) {
    struct Collision collision;
    collision.horizontal_collision = false;
    collision.vertical_collision = false;
    if (r1.y <= r2.y + r2.height && r1.y + r1.height >= r2.y) {
        if (r1.x <= r2.x + r2.width && r1.x + r1.width >= r2.x) {
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

void EntityManager::add_guns_bullets_to_snapshot(Snapshot &snapshot) {
    int i = 0;
    for(auto const&[id, gun] : guns){
        snapshot.guns[i] = gun;
        ++i;
    }
    snapshot.guns_quantity = i;
    i = 0;
    for(auto const&[id, bullet] : bullets){
        snapshot.bullets[i] = bullet;
        ++i;
    }
    snapshot.bullets_quantity = i;
}

EntityManager::~EntityManager() {}

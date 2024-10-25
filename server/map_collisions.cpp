#include "map_collisions.h"

#include "common/shared_constants.h"
#include <iostream>

const int16_t NEAR_CELLS = 3;

MapCollisions::MapCollisions() : collectable_id() {}

void MapCollisions::add_block(int16_t x, int16_t y) {
    Rectangle rectangle = {x, y, BLOCK_SIZE, BLOCK_SIZE};
    blocks[y / BLOCK_SIZE].push_back(rectangle);
}

uint32_t MapCollisions::get_and_inc_collectable_id() { return ++collectable_id; }

void MapCollisions::add_gun(Gun& gun) {
    GunEntity gun_entity(gun);
    guns[gun.gun_id] = gun_entity;
}

void MapCollisions::add_gun(GunEntity& gun) {
    if (gun.type == None) {
        return;
    }
    guns[gun.id] = gun;
}

GunEntity MapCollisions::pickup(const Rectangle &duck) {
    Rectangle gun_r = {0, 0, COLLECTABLE_HITBOX_WIDTH, COLLECTABLE_HITBOX_HEIGHT};
    GunEntity new_gun;
    for (auto const& [id, gun] : guns) {
        gun_r.x = gun.x;
        gun_r.y = gun.y;
        Collision collision = rectangles_collision(duck, gun_r);
        if(collision.horizontal_collision && collision.vertical_collision) {
            new_gun = guns[id];
            guns.erase(id);
            return new_gun;
        }
    }
    return new_gun;
}

struct Collision MapCollisions::check_near_blocks_collision(struct Rectangle& entity, int16_t new_x,
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

struct Collision MapCollisions::rectangles_collision(const struct Rectangle& r1,
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

void MapCollisions::add_guns_to_snapshot(Snapshot& snapshot) {
    for (auto &[id, gun] : guns) {
        Gun snapshot_gun = {gun.id, gun.type, gun.x, gun.y};
        snapshot.guns.push_back(snapshot_gun);
    }
}

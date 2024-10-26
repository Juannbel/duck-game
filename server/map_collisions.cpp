#include "map_collisions.h"

#include "common/shared_constants.h"
#include <iostream>

const int16_t NEAR_CELLS = 3;

MapCollisions::MapCollisions() : collectable_id() {}

void MapCollisions::add_block(float x, float y) {
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
        gun_r.coords.x = gun.x;
        gun_r.coords.y = gun.y;
        Collision collision = rectangles_collision(duck, gun_r);
        if(collision.horizontal_collision && collision.vertical_collision) {
            new_gun = std::move(guns[id]);
            guns.erase(id);
            return new_gun;
        }
    }
    return new_gun;
}

void MapCollisions::drop_gun(GunEntity &&gun, float x, float y){
    if (gun.type == None) {
        return;
    }
    gun.x = x;
    gun.y = y+DUCK_HITBOX_HEIGHT-COLLECTABLE_HITBOX_HEIGHT;
    add_gun(gun);
    gun.drop();
}

struct Collision MapCollisions::check_near_blocks_collision(struct Rectangle& entity, float new_x,
                                                            float new_y) {
    int16_t row_index = entity.coords.y / BLOCK_SIZE;
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
                final_rec.coords.x = entity.coords.x;
                collision.horizontal_collision = true;
                bool vertical_collision = rectangles_collision(final_rec, block).vertical_collision;
                if (vertical_collision) {
                    Coordenades& entity_c = entity.coords;
                    Coordenades& block_c = block.coords;
                    if (new_y > entity_c.y && new_y + entity.height > block_c.y && entity_c.y < block_c.y) {
                        final_rec.coords.y = block_c.y - entity.height;
                    } else if (new_y < block_c.y + block.height && entity_c.y > block_c.y) {
                        final_rec.coords.y = block_c.y+block.height;
                    }
                    collision.vertical_collision = true;
                }
            } else if (aux_collision.vertical_collision) {
                collision.vertical_collision = true;
            }
        }
    }
    collision.last_valid_position = final_rec.coords;
    return collision;
}

struct Collision MapCollisions::rectangles_collision(const struct Rectangle& r1,
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
            if (r1_c.y == r2_c.y+r2.height) {
                collision.vertical_collision = false;
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

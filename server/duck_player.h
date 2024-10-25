#ifndef DUCK_PLAYER_H
#define DUCK_PLAYER_H

#include "common/shared_constants.h"
#include "common/snapshot.h"

#include "action.h"
#include "map_collisions.h"

class DuckPlayer {
private:
    Duck status;
    GunEntity equipped_gun;
    uint8_t it_jumping;
    uint8_t it_flapping;
    float x;
    float y;
    Rectangle hitbox;
    MapCollisions& map_collisions;

    void status_after_move(struct Collision& collision);
    uint32_t pickup();
public:
    DuckPlayer(MapCollisions& map_collisions);
    void set_coordenades_and_id(int16_t x, int16_t y, uint8_t id);
    uint32_t update_status(const Command& command);
    void move_duck();
    Duck get_status();
};

#endif

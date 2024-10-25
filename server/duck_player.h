#ifndef DUCK_PLAYER_H
#define DUCK_PLAYER_H

#include "common/shared_constants.h"
#include "common/snapshot.h"

#include "action.h"
#include "map_collisions.h"

class DuckPlayer {
private:
    Duck status;
    uint8_t ammo;
    uint8_t it_jumping;
    uint8_t it_flapping;
    float x;
    float y;
    Rectangle hitbox;
    MapCollisions& map_collisions;

    void status_after_move(struct Collision& collision);

public:
    DuckPlayer(MapCollisions& map_collisions);
    void set_coordenades_and_id(int16_t x, int16_t y, uint8_t id);
    void update_status(const Command& command);
    void move_duck();
    Duck get_status();
};

#endif

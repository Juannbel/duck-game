#ifndef DUCK_PLAYER_H
#define DUCK_PLAYER_H

#include "common/shared_constants.h"
#include "common/snapshot.h"

#include "action.h"
#include "entitys_manager.h"

class DuckPlayer {
private:
    Duck status;
    uint8_t ammo;
    uint8_t it_jumping;
    uint8_t it_flapping;
    bool try_pickup;

    void status_after_move(struct Collision& collision);

public:
    float x;
    float y;
    DuckPlayer();
    void set_coordenades_and_id(int16_t x, int16_t y, uint8_t id);
    void update_status(const Command& command);
    void move_duck(EntityManager& entity_manager);
    void pickup(EntityManager& entity_manager);
    Duck get_status();
};

#endif

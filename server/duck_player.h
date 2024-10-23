#ifndef DUCK_PLAYER_H
#define DUCK_PLAYER_H

#include "common/snapshot.h"
#include "common/shared_constants.h"
#include "action.h"
#include "entitys_manager.h"

class DuckPlayer{
    private:
    Duck status;
    uint8_t ammo;
    uint8_t it_jumping;
    uint8_t it_flapping;

    void update_duck_status(struct Collision &collision);

    public:
    float x;
    float y;
    DuckPlayer();
    Duck set_coordenades_and_id(int16_t x, int16_t y, uint8_t id);
    void actualice_status(const Command &command);
    Duck move_duck(EntityManager &entity_manager);

};

#endif

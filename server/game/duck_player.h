#ifndef DUCK_PLAYER_H
#define DUCK_PLAYER_H

#include "common/shared_constants.h"
#include "common/snapshot.h"

#include "collectables_manager.h"
#include "collisions.h"

class DuckPlayer {
private:
    Duck status;
    GunEntity equipped_gun;
    uint8_t it_jumping;
    uint8_t it_flapping;
    bool ready_to_jump;
    Rectangle hitbox;
    CollisionChecks& collisions;
    CollectablesManager& collectables;

    void status_after_move(struct Collision& collision);
public:
    DuckPlayer(CollectablesManager& collectables, CollisionChecks &collisions);
    void set_coordenades_and_id(int16_t x, int16_t y, uint8_t id);
    void move_duck();
    void run(bool right);
    void stop_running();
    void shoot();
    void stop_shooting();
    void lay_down();
    void stand_up();
    void jump();
    void stop_jump();
    uint32_t drop_and_pickup();
    Duck get_status();
};

#endif

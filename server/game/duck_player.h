#ifndef DUCK_PLAYER_H
#define DUCK_PLAYER_H

#include <cstdint>
#include <memory>

#include "common/shared_constants.h"
#include "common/snapshot.h"

#include "collectables_manager.h"
#include "collisions.h"
#include "gun_entity.h"

class DuckPlayer {
private:
    Duck status;
    uint8_t it_jumping;
    uint8_t it_flapping;
    uint8_t it_since_hit;
    bool ready_to_jump;
    Rectangle hitbox;
    CollisionChecks& collisions;
    CollectablesManager& collectables;
    std::shared_ptr<GunEntity> equipped_gun;

    void status_after_move(struct Collision& collision);
    void die();

public:
    DuckPlayer(CollectablesManager& collectables, CollisionChecks& collisions);
    void set_coordenades_and_id(int16_t x, int16_t y, uint8_t id);
    void move_duck();
    void run(bool right);
    void stop_running();
    void shoot();
    void stop_shooting();
    void update_gun_status();
    void equip_armor();
    void equip_helmet();
    void lay_down();
    void stand_up();
    void jump();
    void stop_jump();
    uint32_t drop_and_pickup();
    void drop_collectable();
    bool get_hit(Rectangle& bullet, uint8_t damage);
    void knockback();
    Duck get_status();
    Rectangle& get_hitbox();
};

#endif

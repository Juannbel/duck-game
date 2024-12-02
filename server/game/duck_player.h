#ifndef DUCK_PLAYER_H
#define DUCK_PLAYER_H

#include <cstdint>
#include <memory>
#include <string>

#include "common/shared_constants.h"
#include "common/snapshot.h"

#include "collectables_manager.h"
#include "collisions.h"
#include "gun_entity.h"

class GameOperator;

class Cheats {
public:
    bool flying;
    bool infiniteHP;
};

class DuckPlayer {
private:
    Duck status;
    Cheats cheats_on;
    uint8_t it_jumping;
    uint8_t it_flapping;
    uint8_t it_sliding;
    bool ready_to_jump;
    Rectangle hitbox;
    int16_t spawn_x;
    int16_t spawn_y;
    CollisionChecks& collisions;
    CollectablesManager& collectables;
    std::shared_ptr<GunEntity> equipped_gun;

    void status_after_move(struct Collision& collision);
    Collision move_sliding();
    Collision normal_duck_move();
    void die();
    void infinite_hp();
    void fly_mode();
    void revive();
    void equip_armor();
    void equip_helmet();
    bool fall_from_platform();
    float calculate_move_y_jumping();
    float calculate_move_y_flying();
    void knockback();

public:
    DuckPlayer(CollectablesManager& collectables, CollisionChecks& collisions, int16_t x, int16_t y,
               uint8_t id, const std::string& name);
    void move_duck();
    void run(bool right);
    void stop_running();
    void shoot();
    void stop_shooting();
    void update_gun_status();
    void lay_down();
    void stand_up();
    void face_up();
    void stop_face_up();
    void jump();
    void stop_jump();
    uint32_t drop_and_pickup();
    void drop_collectable();
    bool get_hit(const Rectangle& bullet, uint8_t damage);
    void slide();
    Duck get_status();
    const Coordenades& get_coords();


    friend class GameOperator;
};

#endif

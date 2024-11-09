#ifndef GUN_ENTITY_H
#define GUN_ENTITY_H

#include <cstdint>

#include "common/snapshot.h"
#include "server/game/collisions.h"

#include "bullets_manager.h"

class DuckPlayer;  // Forward declaration para evitar dependencia circular

class GunEntity {
protected:
    uint32_t id;
    GunType type;

    Rectangle hitbox;
    bool facing_right;
    uint8_t it_mooving;
    uint8_t ammo;
    uint16_t range;
    uint8_t bullets_to_shoot;
    uint8_t shooted_bullets;
    int16_t initial_angle;
    int16_t inaccuracy;
    bool trigger_pulled;
    bool ready_to_shoot;
    uint8_t it_since_shoot;
    uint8_t it_to_shoot;
    uint8_t it_to_reload;
    uint8_t it_reloading;
    BulletManager* bullets;
    CollisionChecks& collisions;

    int16_t get_rand_angle();
    void add_bullet(DuckPlayer& player);

public:
    GunEntity(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);

    virtual void start_shooting() {
        it_since_shoot = trigger_pulled ? it_since_shoot : 1;
        trigger_pulled = true;
    }

    virtual void update_bullets(DuckPlayer& player) = 0;


    virtual void stop_shooting() {
        trigger_pulled = false;
        it_since_shoot = it_to_shoot;
    }

    void trhow(bool facing_right);
    void check_movement();
    void destroy();
    void set_new_coords(float x, float y);
    Gun get_gun_info();
    const Rectangle& get_hitbox();

    GunEntity(const GunEntity&) = delete;
    GunEntity& operator=(const GunEntity&) = delete;

    GunEntity(GunEntity&&) = delete;
    GunEntity& operator=(GunEntity&&) = delete;

    virtual ~GunEntity() {}
};

#endif

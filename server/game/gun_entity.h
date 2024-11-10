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
    uint16_t it_since_shoot;
    uint16_t it_to_shoot;
    uint16_t it_to_reload;
    uint16_t it_reloading;
    BulletManager* bullets;
    CollisionChecks& collisions;

    int16_t get_rand_angle();
    Rectangle get_bullet_hitbox(Duck& status);
    void add_bullet(DuckPlayer& player);
    void check_movement();
protected:
    virtual void explode_grenade() {};

public:
    GunEntity(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);

    virtual void start_shooting() {
        it_since_shoot = trigger_pulled ? it_since_shoot : 1;
        trigger_pulled = true;
    }

    virtual void stop_shooting() {
        trigger_pulled = false;
        it_since_shoot = it_to_shoot;
    }

    virtual void update_bullets(DuckPlayer& player) = 0;
    virtual void update_status();

    virtual void trhow(bool facing_right);
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

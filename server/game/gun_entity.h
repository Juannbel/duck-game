#ifndef GUN_ENTITY_H
#define GUN_ENTITY_H

#include <cstdint>

#include "common/snapshot.h"
#include "server/game/collisions.h"

#include "bullets_manager.h"

class GameOperator;

class GunEntity {
private:
    bool infinite_ammo_activated;
    void infinite_ammo();

protected:
    uint32_t id;
    GunType type;

    Rectangle hitbox;
    bool facing_right;
    bool stuck;
    uint8_t it_mooving;
    uint8_t ammo;
    float knockback;
    uint8_t bullets_to_shoot;
    uint8_t shooted_bullets;
    int16_t initial_angle;
    int16_t inaccuracy;
    bool trigger_pulled;
    bool ready_to_shoot;
    uint16_t it_since_shoot;
    uint16_t it_to_shoot;
    uint16_t it_reloading;
    BulletManager* bullets;
    CollisionChecks& collisions;

    int16_t get_rand_angle();
    void add_bullet(const Rectangle& player_hb, int16_t angle, bool facing_right, bool facing_up);
    void check_movement();
    int16_t calculate_initial_angle(bool facing_right, bool facing_up);
    virtual void explode_grenade() {}

public:
    GunEntity(Gun& gun, BulletManager* bullets, CollisionChecks& collisions);

    virtual void start_shooting();

    virtual void stop_shooting();

    virtual bool update_bullets(const Rectangle& player_hb, bool facing_right, bool facing_up) = 0;
    virtual void update_status();

    virtual void throw_gun(bool facing_right);
    void destroy();
    void drop_gun(float x, float y);
    float get_knockback();
    Gun get_gun_info();
    const Rectangle& get_hitbox();
    bool empty();

    friend class GameOperator;

    GunEntity(const GunEntity&) = delete;
    GunEntity& operator=(const GunEntity&) = delete;

    GunEntity(GunEntity&&) = delete;
    GunEntity& operator=(GunEntity&&) = delete;

    virtual ~GunEntity() {}
};

#endif

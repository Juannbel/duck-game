#ifndef GUN_ENTITY_H
#define GUN_ENTITY_H

#include <cstdint>

#include "common/snapshot.h"
#include "bullets_manager.h"

class DuckPlayer; // Forward declaration para evitar dependencia circular

class GunEntity{
protected:
    uint32_t id;
    GunType type;

    float x;
    float y;   
    uint8_t ammo;
    bool trigger_pulled;
    bool ready_to_shoot;
    uint8_t it_since_shoot;
    BulletManager* bullets;

public:
    explicit GunEntity(BulletManager* bullets);
    GunEntity(Gun& gun, BulletManager* bullets);
    explicit GunEntity(GunEntity&&);
    GunEntity& operator=(GunEntity&&);

    virtual void start_shooting() { 
        it_since_shoot = trigger_pulled ? it_since_shoot : 0;
        trigger_pulled = true;
    }
    virtual void stop_shooting() {
        trigger_pulled = false;
    }
    virtual void update_bullets(DuckPlayer& player) = 0;

    void drop();
    void set_new_coords(float x, float y);
    Gun get_gun_info();

    virtual ~GunEntity() {}
};

#endif

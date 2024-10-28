#ifndef GUN_ENTITY_H
#define GUN_ENTITY_H

#include <cstdint>

#include "common/snapshot.h"
#include "collisions.h"
#include "bullets_manager.h"
#include "duck_player.h"
#include <map>
#include <set>

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
    GunEntity(CollisionChecks& collisions, BulletManager* bullets);
    GunEntity(Gun& gun, CollisionChecks& collisions, BulletManager* bullets);
    explicit GunEntity(GunEntity&&);
    GunEntity& operator=(GunEntity&&);

    virtual void start_shooting() {}
    virtual void stop_shooting() {}
    virtual void update_bullets(const Duck& status) { 
        if(status.is_shooting)
            trigger_pulled = true; 
    }

    void drop();
    void set_new_coords(float x, float y);
    Gun get_gun_info();

    virtual ~GunEntity() {}
};

#endif

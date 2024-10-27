#ifndef GUN_ENTITY_H
#define GUN_ENTITY_H

#include <cstdint>

#include "common/snapshot.h"
#include <map>
#include <set>

class GunEntity{
private:
    uint32_t id;
    GunType type;

    int16_t x;
    int16_t y;
protected:   
    uint8_t ammo;
    bool trigger_pulled;
    //std::map<uint32_t, Bullet> *bullets;
    std::set<uint32_t> my_bullets;
public:
    GunEntity();
    explicit GunEntity(Gun& gun);

    virtual void start_shooting() {}
    virtual void stop_shooting() {}

    void drop();
    void set_new_coords(int16_t x, int16_t y);
    Gun get_gun_info();

    virtual ~GunEntity() {}
};

#endif

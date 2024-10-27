#ifndef GUN_ENTITY_H
#define GUN_ENTITY_H

#include <cstdint>

#include "common/snapshot.h"

class GunEntity{
private:
    uint32_t id;
    GunType type;

    int16_t x;
    int16_t y;
    
    uint8_t ammo;
    bool trigger_pulled;
public:
    GunEntity();
    explicit GunEntity(Gun& gun);
    void drop();
    void set_new_coords(int16_t x, int16_t y);
    Gun get_gun_info();
};

#endif

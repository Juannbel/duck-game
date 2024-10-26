#ifndef GUN_ENTITY_H
#define GUN_ENTITY_H

#include <cstdint>

#include "common/snapshot.h"

class GunEntity{
    public:
    uint32_t id;
    GunType type;

    int16_t x;
    int16_t y;
    
    uint8_t ammo;
    bool trigger_pulled;
    GunEntity();
    explicit GunEntity(Gun& gun);
    void drop();
};

#endif

#include "gun_entity.h"

GunEntity::GunEntity() : 
        id(), type(), x(), y(), ammo(), trigger_pulled() {}

GunEntity::GunEntity(Gun& gun) : 
    id(gun.gun_id), type(gun.type), x(gun.x), y(gun.y), ammo(), trigger_pulled(){}

void GunEntity::drop() {
    id = 0;
    type = None;
    x = 0;
    y = 0;
    ammo = 0;
    trigger_pulled = false;
}

void GunEntity::set_new_coords(int16_t x, int16_t y) {
    this->x = x;
    this->y = y;
}

Gun GunEntity::get_gun_info() {
    Gun gun_info = {id, type, x, y};
    return gun_info;
}

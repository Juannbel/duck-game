#include "gun_entity.h"

#include <cstdint>


GunEntity::GunEntity(BulletManager* bullets):
        id(),
        type(),
        x(),
        y(),
        ammo(),
        trigger_pulled(),
        ready_to_shoot(),
        it_since_shoot(),
        bullets(bullets) {}

GunEntity::GunEntity(Gun& gun, BulletManager* bullets):
        id(gun.gun_id),
        type(gun.type),
        x(gun.x),
        y(gun.y),
        ammo(),
        trigger_pulled(),
        ready_to_shoot(),
        it_since_shoot(),
        bullets(bullets) {}

GunEntity::GunEntity(GunEntity&& old):
        id(old.id),
        type(old.type),
        x(old.x),
        y(old.y),
        ammo(old.ammo),
        trigger_pulled(old.trigger_pulled),
        ready_to_shoot(old.ready_to_shoot),
        it_since_shoot(old.it_since_shoot),
        bullets(old.bullets) {}

GunEntity& GunEntity::operator=(GunEntity&& old) {
    id = old.id;
    type = old.type;
    x = old.x;
    y = old.y;
    ammo = old.ammo;
    trigger_pulled = old.trigger_pulled;
    bullets = old.bullets;
    return *this;
}


void GunEntity::drop() {
    id = 0;
    type = None;
    x = 0;
    y = 0;
    ammo = 0;
    trigger_pulled = false;
}

void GunEntity::set_new_coords(float x, float y) {
    this->x = x;
    this->y = y;
}

Gun GunEntity::get_gun_info() {
    Gun gun_info = {id, type, static_cast<int16_t>(x), static_cast<int16_t>(y)};
    return gun_info;
}

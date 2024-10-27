#include "bullet.h"
#include <cassert>
#include "common/snapshot.h"
#include "common/shared_constants.h"

RenderableBullet::RenderableBullet(uint32_t id, GunType type) :
    id(id), type(type), position(0, 0) {}

uint32_t RenderableBullet::get_id() {
    return id;
}

void RenderableBullet::update(const Bullet& snapshot) {
    assert(snapshot.type != None);
    position.x = snapshot.x;
    position.y = snapshot.y;
}

void RenderableBullet::render(SDL2pp::Renderer& renderer, Camera& camera) {
    // como falta ajustar los offsets, dibujo la hitbox
    SDL2pp::Rect hitbox(position.x, position.y, BULLET_HITBOX_WIDTH, BULLET_HITBOX_HEIGHT);
    camera.transform_rect(hitbox);
    renderer.SetDrawColor(255, 0, 0, 255);
    renderer.FillRect(hitbox);
    renderer.DrawRect(hitbox);
    renderer.SetDrawColor(0, 0, 0, 255);

    // animation.render(renderer, camera, position);
}

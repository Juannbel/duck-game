#include "collectable.h"
#include "SDL2pp/Rect.hh"
#include "common/snapshot.h"
#include "common/shared_constants.h"

#include <yaml-cpp/yaml.h>

RenderableCollectable::RenderableCollectable(uint32_t id,
    SDL2pp::Texture* sprite, FrameData frame_data) :
    id(id), animation(*sprite, {frame_data}, 1, false), position(0, 0) {}

uint32_t RenderableCollectable::get_id() {
    return id;
}

void RenderableCollectable::update_from_snapshot(const Gun& snapshot) {
    assert(snapshot.gun_id == id);
    position.x = snapshot.x;
    position.y = snapshot.y;
}

void RenderableCollectable::render(SDL2pp::Renderer& renderer, Camera& camera) {
    // como falta ajustar los offsets, dibujo la hitbox
    SDL2pp::Rect hitbox(position.x, position.y, COLLECTABLE_HITBOX_WIDTH, COLLECTABLE_HITBOX_HEIGHT);
    camera.transform_rect(hitbox);
    renderer.DrawRect(hitbox);


    animation.render(renderer, camera, position);
}

#include "feathers.h"
#include "client/animation_data_provider.h"
#include "client/textures_provider.h"

RenderableFeathers::RenderableFeathers(uint8_t duck_id) :
    animation(*TexturesProvider::get_texture("feathers"),
        AnimationDataProvider::get_animation_data("feathers_" + std::to_string(duck_id) + "_damaged")),
    position(0, 0), never_damaged(true) {}

void RenderableFeathers::update(const Duck& duck) {
    if (duck.is_damaged) {
        never_damaged = false;
        position.x = duck.x;
        position.y = duck.y;
        animation.restart();
    }

    animation.update();
}


void RenderableFeathers::render(SDL2pp::Renderer& renderer, Camera& camera) {
    if (never_damaged) return;
    animation.render(renderer, camera, position);
}

void RenderableFeathers::skip_frames(uint8_t frames) {
    animation.skip_frames(frames);
}

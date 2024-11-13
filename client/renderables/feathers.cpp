#include "feathers.h"
#include <algorithm>

#include "client/animation_data_provider.h"
#include "client/textures_provider.h"
#include "common/config.h"

RenderableFeathers::RenderableFeathers(uint8_t duck_id):
        render_iterations(Config::get_client_fps()),
        animation(*TexturesProvider::get_texture("feathers"),
                  AnimationDataProvider::get_animation_data("feathers_" + std::to_string(duck_id) +
                                                            "_damaged")),
        position(0, 0),
        last_hp(Config::get_initial_duck_hp()),
        iterations_left(0) {}

void RenderableFeathers::update(const Duck& duck) {
    if (duck.duck_hp < last_hp) {
        last_hp = duck.duck_hp;
        position.x = duck.x;
        position.y = duck.y;
        iterations_left = render_iterations;
        animation.restart();
    }

    animation.update();
}


void RenderableFeathers::render(SDL2pp::Renderer& renderer, Camera& camera) {
    if (iterations_left > 0) {
        iterations_left--;
        animation.render(renderer, camera, position);
    }
}

void RenderableFeathers::skip_frames(uint8_t frames) {
    frames = std::min(frames, iterations_left);
    iterations_left -= frames;
    animation.skip_frames(frames);
}

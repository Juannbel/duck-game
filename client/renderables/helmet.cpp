#include "helmet.h"

#include "client/animation_data_provider.h"
#include "client/textures_provider.h"

RenderableHelmet::RenderableHelmet(): curr_animation(nullptr), position(0, 0), facing_right(true) {
    animations["standing"] =
            new Animation(*TexturesProvider::get_texture("helmet"),
                          AnimationDataProvider::get_animation_data("helmet_standing"));

    animations["running"] =
            new Animation(*TexturesProvider::get_texture("helmet"),
                          AnimationDataProvider::get_animation_data("helmet_running"));

    animations["laying"] =
            new Animation(*TexturesProvider::get_texture("helmet"),
                          AnimationDataProvider::get_animation_data("helmet_laying"));
}

void RenderableHelmet::update(const Duck& duck) {
    Animation* prev_animation = curr_animation;

    position.x = duck.x;
    position.y = duck.y;

    if (duck.is_dead)
        return;

    if (!duck.helmet_equiped) {
        curr_animation = nullptr;
        return;
    } else if (duck.is_jumping || duck.is_falling) {
        curr_animation = animations["standing"];
    } else if (duck.is_running) {
        curr_animation = animations["running"];
    } else if (duck.is_laying) {
        curr_animation = animations["laying"];
    } else {
        curr_animation = animations["standing"];
    }

    if (prev_animation != curr_animation) {
        curr_animation->restart();
    }

    curr_animation->update();

    facing_right = duck.facing_right;
}

void RenderableHelmet::render(SDL2pp::Renderer& renderer, Camera& camera) {
    if (!curr_animation)
        return;
    curr_animation->render(renderer, camera, position, facing_right, 0);
}

void RenderableHelmet::skip_frames(uint8_t frames) {
    if (!curr_animation)
        return;
    curr_animation->skip_frames(frames);
}

RenderableHelmet::~RenderableHelmet() {
    for (auto& pair: animations) {
        delete pair.second;
    }
    animations.clear();
}

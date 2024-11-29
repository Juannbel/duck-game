#include "helmet.h"

#include "client/animation_data_provider.h"
#include "client/textures_provider.h"

RenderableHelmet::RenderableHelmet(SDL2pp::Renderer& renderer): curr_animation(nullptr), position(0, 0), facing_right(true) {
    auto& animation_data_provider = AnimationDataProvider::get_instance();
    auto& textures_provider = TexturesProvider::get_instance(renderer);

    animations["standing"] =
            new Animation(*textures_provider.get_texture("helmet"),
                          animation_data_provider.get_animation_data("helmet_standing"));

    animations["running"] =
            new Animation(*textures_provider.get_texture("helmet"),
                          animation_data_provider.get_animation_data("helmet_running"));

    animations["laying"] =
            new Animation(*textures_provider.get_texture("helmet"),
                          animation_data_provider.get_animation_data("helmet_laying"));
}

void RenderableHelmet::update(const Duck& duck) {
    Animation* prev_animation = curr_animation;

    position.x = duck.x;
    position.y = duck.y;

    if (!duck.helmet_equiped || duck.is_dead) {
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

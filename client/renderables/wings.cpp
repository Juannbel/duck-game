#include "wings.h"

#include "client/animation_data_provider.h"
#include "client/textures_provider.h"
#include "common/snapshot.h"

RenderableWings::RenderableWings(uint8_t duck_id, SDL2pp::Renderer& renderer):
        duck_id(duck_id), position(0, 0), facing_right(true) {

    auto& anim_provider = AnimationDataProvider::get_instance();
    auto& text_provider = TexturesProvider::get_instance(renderer);

    load_animation("standing", anim_provider, text_provider);
    load_animation("walking", anim_provider, text_provider);
    load_animation("falling", anim_provider, text_provider);
    load_animation("armed", anim_provider, text_provider);
    load_animation("laying", anim_provider, text_provider);
    load_animation("flapping", anim_provider, text_provider);
    load_animation("dead", anim_provider, text_provider);

    curr_animation = animations["standing"];
}

void RenderableWings::load_animation(const std::string& name, AnimationDataProvider& anim_provider,
                                     TexturesProvider& text_provider) {
    animations[name] = new Animation(
            *text_provider.get_texture("wings"),
            anim_provider.get_animation_data("wings_" + std::to_string(duck_id) + "_" + name));
}

void RenderableWings::update(const Duck& duck) {
    Animation* prev_animation = curr_animation;

    position.x = duck.x;
    position.y = duck.y;

    facing_right = duck.facing_right;

    if (duck.is_dead) {
        curr_animation = animations["dead"];
    } else if (duck.is_laying) {
        curr_animation = animations["laying"];
    } else if (duck.gun != GunType::None) {
        curr_animation = animations["armed"];
    } else if (duck.is_flapping) {
        curr_animation = animations["flapping"];
    } else if (duck.is_falling) {
        curr_animation = animations["falling"];
    } else if (duck.is_running) {
        curr_animation = animations["walking"];
    } else {
        curr_animation = animations["standing"];
    }

    if (prev_animation != curr_animation) {
        curr_animation->restart();
    }

    curr_animation->update();
}

void RenderableWings::render(SDL2pp::Renderer& renderer, Camera& camera) {
    curr_animation->render(renderer, camera, position, facing_right);
}

void RenderableWings::skip_frames(uint8_t frames) { curr_animation->skip_frames(frames); }

RenderableWings::~RenderableWings() {
    for (auto& animation: animations) {
        delete animation.second;
    }
}

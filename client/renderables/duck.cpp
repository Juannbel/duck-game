#include "duck.h"

#include <string>

#include <yaml-cpp/yaml.h>

#include "client/animation_data_provider.h"
#include "client/renderables/feathers.h"
#include "client/textures_provider.h"
#include "common/shared_constants.h"

RenderableDuck::RenderableDuck(uint8_t duck_id, SDL2pp::Renderer& renderer):
        duck_id(duck_id),
        gun(renderer),
        helmet(renderer),
        armor(renderer),
        wings(duck_id, renderer),
        feathers(duck_id, renderer),
        position(50, 50),
        is_facing_right(true),
        is_alive(true) {
    auto& animation_data_provider = AnimationDataProvider::get_instance();
    auto& textures_provider = TexturesProvider::get_instance(renderer);
    load_animations(animation_data_provider, textures_provider);
    curr_animation = animations["standing"];
}

void RenderableDuck::load_animation(const std::string& animation_name, AnimationDataProvider& anim_prov, TexturesProvider& text_prov) {
    animations[animation_name] =
            new Animation(*text_prov.get_texture("duck"),
                          anim_prov.get_animation_data(
                                  "duck_" + std::to_string(duck_id) + "_" + animation_name));
}

uint8_t RenderableDuck::get_id() { return duck_id; }

void RenderableDuck::load_animations(AnimationDataProvider& anim_prov, TexturesProvider& text_prov) {
    load_animation("dead", anim_prov, text_prov);
    load_animation("falling", anim_prov, text_prov);
    load_animation("jumping", anim_prov, text_prov);
    load_animation("laying", anim_prov, text_prov);
    load_animation("standing", anim_prov, text_prov);
    load_animation("walking", anim_prov, text_prov);
    load_animation("facing_up", anim_prov, text_prov);
}

void RenderableDuck::update(const Duck& duck) {
    Animation* prev_animation = curr_animation;

    position.x = duck.x;
    position.y = duck.y;

    is_facing_right = duck.facing_right;

    if (duck.is_dead) {
        curr_animation = animations["dead"];
        is_alive = false;
    } else if (duck.is_jumping) {
        curr_animation = animations["jumping"];
    } else if (duck.is_falling) {
        curr_animation = animations["falling"];
    } else if (duck.is_running) {
        curr_animation = animations["walking"];
    } else if (duck.is_laying) {
        curr_animation = animations["laying"];
    } else if (duck.facing_up) {
        curr_animation = animations["facing_up"];
    } else {
        curr_animation = animations["standing"];
    }

    if (prev_animation != curr_animation) {
        curr_animation->restart();
    }

    curr_animation->update();

    helmet.update(duck);
    armor.update(duck);
    gun.update(duck);
    wings.update(duck);
    feathers.update(duck);
}

void RenderableDuck::render(SDL2pp::Renderer& renderer, Camera& camera) {
    curr_animation->render(renderer, camera, position, is_facing_right);

    feathers.render(renderer, camera);
    helmet.render(renderer, camera);
    armor.render(renderer, camera);
    gun.render(renderer, camera);
    wings.render(renderer, camera);
}

void RenderableDuck::skip_frames(uint8_t frames) {
    curr_animation->skip_frames(frames);
    feathers.skip_frames(frames);
    helmet.skip_frames(frames);
    armor.skip_frames(frames);
    wings.skip_frames(frames);
}

bool RenderableDuck::is_dead() { return !is_alive; }

SDL2pp::Rect RenderableDuck::get_bounding_box() {
    return SDL2pp::Rect(position.x, position.y, DUCK_HITBOX_WIDTH, DUCK_HITBOX_HEIGHT);
}

RenderableDuck::~RenderableDuck() {
    for (auto& animation: animations) {
        delete animation.second;
    }
}

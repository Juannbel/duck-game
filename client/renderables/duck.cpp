#include "duck.h"

#include <yaml-cpp/yaml.h>
#include <string>

#include "client/animation_data_provider.h"
#include "client/textures_provider.h"
#include "common/shared_constants.h"

RenderableDuck::RenderableDuck(uint8_t duck_id):
            duck_id(duck_id), position(0, 0), is_facing_right(true), is_alive(true) {
        load_animations();
        curr_animation = animations["standing"];
    }

void RenderableDuck::load_animation(const std::string& animation_name) {
    animations[animation_name] =
        new Animation(
            *TexturesProvider::getTexture("duck"),
            AnimationDataProvider::get_animation_data("duck_" + std::to_string(duck_id) + "_" + animation_name));
}

void RenderableDuck::load_animations() {
    load_animation("dead");
    load_animation("falling");
    load_animation("jumping");
    load_animation("laying");
    load_animation("standing");
    load_animation("walking");
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
    } else {
        curr_animation = animations["standing"];
    }

    if (prev_animation != curr_animation) {
        curr_animation->restart();
    }

    curr_animation->update();

    gun.update(duck);
    helmet.update(duck);
}

void RenderableDuck::render(SDL2pp::Renderer& renderer, Camera& camera) {
    // SDL2pp::Rect hitbox = SDL2pp::Rect(position.x, position.y, DUCK_HITBOX_WIDTH, DUCK_HITBOX_HEIGHT);
    // camera.transform_rect(hitbox);
    // renderer.DrawRect(hitbox);

    curr_animation->render(renderer, camera, position, is_facing_right);

    if (!is_alive) {
        return;
    }

    gun.render(renderer, camera);
    helmet.render(renderer, camera);
}

void RenderableDuck::skip_frames(uint8_t frames) { curr_animation->skip_frames(frames); }

bool RenderableDuck::is_dead() { return !is_alive; }

SDL2pp::Rect RenderableDuck::get_bounding_box() { return SDL2pp::Rect(position.x, position.y, DUCK_HITBOX_WIDTH, DUCK_HITBOX_HEIGHT); }

RenderableDuck::~RenderableDuck() {
    for (auto& animation: animations) {
        delete animation.second;
    }
}

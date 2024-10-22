#include "duck.h"

#include <yaml-cpp/yaml.h>
#include <vector>

#include "../../common/shared_constants.h"

RenderableDuck::RenderableDuck(SDL2pp::Texture* sprite, const std::string& config_path,
                   SDL2pp::Texture* guns_sprite, const std::string& guns_config):
            gun(guns_sprite, guns_config), position(0, 0), is_facing_right(true), is_alive(true) {
        load_animations(sprite, config_path);
        curr_animation = animations["standing"];
    }

void RenderableDuck::load_animations(SDL2pp::Texture* sprite, const std::string& config_path) {
    YAML::Node config = YAML::LoadFile(config_path);

    int width = config["width"].as<int>();
    int height = config["height"].as<int>();

    for (const auto& animation: config["animations"]) {
        std::string name = animation.first.as<std::string>();
        std::vector<FrameData> frames;

        for (const auto& frame: animation.second["frames"]) {
            int source_x = frame["x"].as<int>();
            int source_y = frame["y"].as<int>();
            int x_offset_right = frame["x_offset_right"].as<int>();
            int x_offset_left = frame["x_offset_left"].as<int>();
            int y_offset = frame["y_offset"].as<int>();

            SDL2pp::Rect rect = SDL2pp::Rect(source_x, source_y, width, height);
            frames.push_back({rect, x_offset_right, x_offset_left, y_offset});
        }
        uint8_t iter_per_frame = animation.second["iter_per_frame"].as<uint8_t>();
        bool loops = animation.second["loops"].as<bool>();
        animations[name] = new Animation(*sprite, frames, iter_per_frame, loops);
    }
}


void RenderableDuck::update() {
    curr_animation->update();
    gun.update();
}

void RenderableDuck::render(SDL2pp::Renderer& renderer, Camera& camera) {
    SDL2pp::Rect hitbox = SDL2pp::Rect(position.x, position.y, DUCK_HITBOX_WIDTH, DUCK_HITBOX_HEIGHT);
    camera.transform_rect(hitbox);
    renderer.DrawRect(hitbox);

    curr_animation->render(renderer, camera, position, is_facing_right);

    if (!is_alive) {
        return;
    }

    gun.render(renderer, camera);
}

void RenderableDuck::skip_frames(uint8_t frames) { curr_animation->skip_frames(frames); }

void RenderableDuck::update_from_snapshot(const Duck& duck) {
    Animation* prev_animation = curr_animation;

    position.x = duck.x;
    position.y = duck.y;

    is_facing_right = duck.facing_right;

    if (duck.duck_hp == 0) {
        curr_animation = animations["dead"];
        is_alive = false;
    } else if (duck.is_jumping) {
        curr_animation = animations["jumping"];
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

    gun.update_from_snapshot(duck);
}

bool RenderableDuck::is_dead() { return !is_alive; }

SDL2pp::Rect RenderableDuck::get_bounding_box() { return SDL2pp::Rect(position.x, position.y, DUCK_HITBOX_WIDTH, DUCK_HITBOX_HEIGHT); }

RenderableDuck::~RenderableDuck() {
    for (auto& animation: animations) {
        delete animation.second;
    }
}
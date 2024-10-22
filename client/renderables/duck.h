#ifndef RENDERABLE_DUCK_H
#define RENDERABLE_DUCK_H

#include <string>
#include <unordered_map>
#include <vector>

#include <SDL2pp/SDL2pp.hh>
#include <SDL_render.h>
#include <yaml-cpp/yaml.h>

#include "client/renderables/equipped_gun.h"
#include "common/snapshot.h"

#include "animation.h"

#define SPRITE_SIZE 20

class RenderableDuck {
private:
    Animation* curr_animation;
    RenderableEquippedGun gun;

    std::unordered_map<std::string, Animation*> animations;
    int x;
    int y;

    bool is_facing_right;
    bool is_alive;

    int source_width;
    int source_height;

    int x_offset;
    int y_offset;

    int hitbox_width;
    int hitbox_height;

public:
    RenderableDuck(SDL2pp::Texture* sprite, const std::string& config_path,
                   SDL2pp::Texture* guns_sprite, const std::string& guns_config):
            gun(guns_sprite, guns_config), x(0), y(0), is_facing_right(true), is_alive(true) {
        load_animations(sprite, config_path);
        curr_animation = animations["standing"];
    }

    void load_animations(SDL2pp::Texture* sprite, const std::string& config_path) {
        YAML::Node config = YAML::LoadFile(config_path);

        int width = config["width"].as<int>();
        int height = config["height"].as<int>();

        source_width = width;
        source_height = height;

        x_offset = config["x_offset"].as<int>();
        y_offset = config["y_offset"].as<int>();

        hitbox_width = config["hitbox_width"].as<int>();
        hitbox_height = config["hitbox_height"].as<int>();

        for (const auto& animation: config["animations"]) {
            std::string name = animation.first.as<std::string>();
            std::vector<SDL2pp::Rect> frames;

            for (const auto& frame: animation.second["frames"]) {
                int source_x = frame["x"].as<int>();
                int source_y = frame["y"].as<int>();

                frames.push_back(SDL2pp::Rect(source_x, source_y, width, height));
            }
            uint8_t iter_per_frame = animation.second["iter_per_frame"].as<uint8_t>();
            bool loops = animation.second["loops"].as<bool>();
            animations[name] = new Animation(*sprite, frames, iter_per_frame, loops);
        }
    }

    void update() {
        curr_animation->update();
        gun.update();
    }

    void render(SDL2pp::Renderer& renderer, Camera& camera) {
        SDL_RendererFlip flip = !is_facing_right ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

        // muestro la hitbox

        SDL2pp::Rect hitbox = SDL2pp::Rect(x, y, hitbox_width, hitbox_height);
        camera.transform_rect(hitbox);
        renderer.DrawRect(hitbox);

        SDL2pp::Rect dst = SDL2pp::Rect(x - x_offset, y - y_offset, source_width, source_height);

        curr_animation->render(renderer, camera, dst, flip);

        if (!is_alive) {
            return;
        }

        SDL2pp::Rect gun_dst = SDL2pp::Rect(x + 2, y - 2, 32, 32);
        if (!is_facing_right) {
            gun_dst.x -= 20;
        }

        gun.render(renderer, camera);
    }

    void skip_frames(uint8_t frames) { curr_animation->skip_frames(frames); }

    void update_from_snapshot(const Duck& duck) {
        Animation* prev_animation = curr_animation;

        x = duck.x;
        y = duck.y;

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

    bool is_dead() { return !is_alive; }

    SDL2pp::Rect get_bounding_box() { return SDL2pp::Rect(x, y, source_width, source_height); }

    ~RenderableDuck() {
        for (auto& animation: animations) {
            delete animation.second;
        }
    }
};

#endif

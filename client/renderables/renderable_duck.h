#ifndef RENDERABLE_DUCK_H
#define RENDERABLE_DUCK_H

#include <SDL2pp/SDL2pp.hh>
#include <SDL_render.h>
#include <string>
#include <unordered_map>
#include "animation.h"
#include "common/snapshot.h"

#include <yaml-cpp/yaml.h>

#define SPRITE_SIZE 25

class RenderableDuck {
private:
    Animation *curr_animation;

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
    RenderableDuck(Texture *sprite, const std::string& config_path) : x(0), y(0), is_facing_right(true), is_alive(true) {
        YAML::Node config = YAML::LoadFile(config_path);

        int width = config["width"].as<int>();
        int height = config["height"].as<int>();

        source_width = width;
        source_height = height;

        x_offset = config["x_offset"].as<int>();
        y_offset = config["y_offset"].as<int>();

        hitbox_width = config["hitbox_width"].as<int>();
        hitbox_height = config["hitbox_height"].as<int>();

        for (const auto& animation : config["animations"]) {
            std::string name = animation.first.as<std::string>();
            std::vector<Rect> frames;
            int x, y;

            for (const auto& frame : animation.second["frames"]) {
                x = frame["x"].as<int>();
                y = frame["y"].as<int>();

                frames.push_back(Rect(x, y, width, height));
            }
            uint8_t iter_per_frame = animation.second["iter_per_frame"].as<uint8_t>();
            bool loops = animation.second["loops"].as<bool>();
            animations[name] = new Animation(*sprite, frames, iter_per_frame, loops);
        }
        
        curr_animation = animations["standing"];
    }

    void update() {
        curr_animation->update();
    }

    void render(SDL2pp::Renderer &renderer, Camera &camera) {
        SDL_RendererFlip flip = !is_facing_right ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

        // Rect dst = SDL2pp::Rect(x, y, source_width * dest_scale, source_height * dest_scale);

        // muestro la hitbox

        Rect hitbox = SDL2pp::Rect(x, y, hitbox_width, hitbox_height);
        camera.transform_rect(hitbox);
        renderer.DrawRect(hitbox);

        Rect dst = SDL2pp::Rect(x - x_offset, y - y_offset, source_width, source_height);

        curr_animation->render(
            renderer, 
            camera,
            dst, 
            flip);
    }

    void skip_frames(uint8_t frames) {
        curr_animation->skip_frames(frames);
    }

    void update_from_snapshot(const Duck& duck) {
        Animation *prev_animation = curr_animation;

        x = duck.x;
        y = duck.y;

        is_facing_right = duck.facing_right;

        if (duck.duck_hp == 0) {
            curr_animation = animations["dead"];
            is_alive = false;
            return;
        } else if (duck.is_jumping) {
            curr_animation = animations["jumping"];
            return;
        } else if (duck.is_running) {
            curr_animation = animations["walking"];
        } else if (duck.is_lying) {
            curr_animation = animations["laying"];
        } else {
            curr_animation = animations["standing"];
        }

        if (prev_animation != curr_animation) {
            curr_animation->restart();
        }
    }

    bool is_dead() {
        return !is_alive;
    }

    SDL2pp::Rect get_bounding_box() {
        return SDL2pp::Rect(x, y, source_width, source_height);
    }

    ~RenderableDuck() {
        for (auto& animation : animations) {
            delete animation.second;
        }
    }

};

#endif
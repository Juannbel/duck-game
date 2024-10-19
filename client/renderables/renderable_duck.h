#ifndef RENDERABLE_DUCK_H
#define RENDERABLE_DUCK_H

#include <SDL2pp/SDL2pp.hh>
#include <SDL_render.h>
#include <string>
#include <unordered_map>
#include "animation.h"
#include "common/snapshot.h"

#include <yaml-cpp/yaml.h>

#define SPRITE_SIZE 100

class RenderableDuck {
public:
    RenderableDuck(Texture *sprite, const std::string& config_path) : x(0), y(0), is_facing_right(true), is_alive(true) {
        YAML::Node config = YAML::LoadFile(config_path);

        int width = config["width"].as<int>();
        int height = config["height"].as<int>();

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

    void render(SDL2pp::Renderer &renderer, Rect &camera, float &scale) {
        SDL_RendererFlip flip = !is_facing_right ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

        curr_animation->render(
            renderer, 
            SDL2pp::Rect(
                (x - camera.x) * scale,
                (y - camera.y) * scale,
                SPRITE_SIZE * scale, SPRITE_SIZE * scale
            ), 
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
        return SDL2pp::Rect(x, y, SPRITE_SIZE, SPRITE_SIZE);
    }

    ~RenderableDuck() {
        for (auto& animation : animations) {
            delete animation.second;
        }
    }

private:
    Animation *curr_animation;

    std::unordered_map<std::string, Animation*> animations;
    int x;
    int y;
    bool is_facing_right;
    bool is_alive;

};

#endif
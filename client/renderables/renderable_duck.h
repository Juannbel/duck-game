#ifndef RENDERABLE_DUCK_H
#define RENDERABLE_DUCK_H

#include <SDL2pp/SDL2pp.hh>
#include <string>
#include <unordered_map>
#include "animation.h"
#include "common/snapshot.h"

#include <yaml-cpp/yaml.h>

class RenderableDuck {
public:
    RenderableDuck(Texture *sprite, const std::string& config_path) : is_facing_right(true), is_running(false), is_alive(true), x(0), y(0) {
        YAML::Node config = YAML::LoadFile(config_path);

        for (const auto& animation : config["animations"]) {
            std::string name = animation.first.as<std::string>();
            std::vector<Rect> frames;
            for (const auto& frame : animation.second["frames"]) {
                frames.push_back(Rect(frame["x"].as<int>(), frame["y"].as<int>(), frame["width"].as<int>(), frame["height"].as<int>()));
            }
            animations[name] = new Animation(*sprite, frames, animation.second["iter_per_frame"].as<uint8_t>());
        }
        
        curr_animation = animations["standing"];
    }

    void update() {
        curr_animation->update();
    }

    void render(SDL2pp::Renderer &renderer) {
        SDL_RendererFlip flip = !is_facing_right ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        curr_animation->render(renderer, SDL2pp::Rect(x, y, 50, 50), flip);
    }

    void skip_frames(uint8_t frames) {
        curr_animation->skip_frames(frames);
    }

    void update_from_snapshot(const Duck& duck) {
        set_running(duck.is_running);
        set_facing_right(duck.facing_right);
        set_position(duck.x, duck.y);
    }

    bool is_dead() {
        return !is_alive;
    }

    SDL2pp::Rect get_bounding_box() {
        return SDL2pp::Rect(x, y, 50, 50);
    }

    ~RenderableDuck() {
        for (auto& animation : animations) {
            delete animation.second;
        }
    }

private:
    Animation *curr_animation;

    std::unordered_map<std::string, Animation*> animations;
    bool is_facing_right;
    bool is_running;
    bool is_alive;
    int x;
    int y;

    void set_running(bool running) {
        if (running == is_running) {
            return;
        }

        this->is_running = running;
        if (running) {
            curr_animation = animations["walking"];
        } else {
            curr_animation = animations["standing"];
        }
        curr_animation->restart();
    }

    void set_facing_right(bool facing_right) {
        this->is_facing_right = facing_right;
    }

    void set_position(int x, int y) {
        this->x = x;
        this->y = y;
    }

};

#endif
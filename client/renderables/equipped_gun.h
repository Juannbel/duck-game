#ifndef RENDERABLE_EQUIPPED_GUN_H
#define RENDERABLE_EQUIPPED_GUN_H

#include <string>
#include <unordered_map>

#include <SDL_render.h>
#include <yaml-cpp/yaml.h>

#include "common/snapshot.h"

#include "animation.h"
class RenderableEquippedGun {
private:
    Animation* curr_animation;

    GunType current_gun;
    std::unordered_map<GunType, Animation*> guns;

    SDL2pp::Point position;

    bool facing_right;
    bool facing_up;

    std::unordered_map<std::string, GunType> string_to_gun;

public:
    RenderableEquippedGun(SDL2pp::Texture* sprite, const std::string& config_path):
            position(0, 0), facing_right(true), facing_up(false) {
        string_to_gun["none"] = None;
        string_to_gun["ak47"] = Ak47;
        string_to_gun["dueling_pistol"] = DuelingPistol;

        YAML::Node config = YAML::LoadFile(config_path);

        int width = config["width"].as<int>();
        int height = config["height"].as<int>();


        for (const auto& gun: config["guns"]) {
            GunType gun_type = string_to_gun[gun.first.as<std::string>()];

            std::vector<FrameData> frames;

            for (const auto& frame: gun.second["frames"]) {
                int source_x = frame["x"].as<int>();
                int source_y = frame["y"].as<int>();
                int x_offset_right = frame["x_offset_right"].as<int>();
                int x_offset_left = frame["x_offset_left"].as<int>();
                int y_offset = frame["y_offset"].as<int>();

                SDL2pp::Rect rect = SDL2pp::Rect(source_x, source_y, width, height);
                frames.push_back({rect, x_offset_right, x_offset_left, y_offset});
            }

            uint8_t iter_per_frame = gun.second["iter_per_frame"].as<uint8_t>();
            bool loops = gun.second["loops"].as<bool>();
            guns[gun_type] = new Animation(*sprite, frames, iter_per_frame, loops);
        }

        current_gun = None;
        curr_animation = guns[None];
    }

    void update() { curr_animation->update(); }

    void update_from_snapshot(const Duck& duck) {
        position.x = duck.x;
        position.y = duck.y;

        facing_right = duck.facing_right;
        facing_up = duck.facing_up;

        curr_animation = guns[duck.gun];
        current_gun = duck.gun;
    }

    void render(SDL2pp::Renderer& renderer, Camera& camera) {
        if (current_gun == None) {
            return;
        }

        float angle = facing_up ? facing_right ? 290 : 70 : 0;
        curr_animation->render(renderer, camera, position, facing_right, angle);

    }

    ~RenderableEquippedGun() {
        for (auto& gun: guns) {
            delete gun.second;
        }
    }
};

#endif

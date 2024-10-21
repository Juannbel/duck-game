#ifndef RENDERABLE_EQUIPPED_GUN_H
#define RENDERABLE_EQUIPPED_GUN_H

#include "animation.h"
#include "common/snapshot.h"
#include <SDL_render.h>
#include <unordered_map>
#include <yaml-cpp/yaml.h>

struct SpriteInfo {
    int width;
    int height;

    int facing_right_x_offset;
    int facing_left_x_offset;
    int y_offset;
};

class RenderableEquippedGun {
private:
    Animation *curr_animation;

    GunType current_gun;
    std::unordered_map<GunType, Animation*> guns;
    std::unordered_map<GunType, SpriteInfo> sprites_info;

    int x;
    int y;

    bool facing_right;
    bool facing_up;

    std::unordered_map<std::string, GunType> string_to_gun;

public:

    RenderableEquippedGun(Texture *sprite, const std::string& config_path) : x(0), y(0), facing_right(true), facing_up(false) {
        string_to_gun["none"] = None;
        string_to_gun["ak47"] = Ak47;
        string_to_gun["dueling_pistol"] = DuelingPistol;

        YAML::Node config = YAML::LoadFile(config_path);

        for (const auto& gun : config["guns"]) {
            GunType gun_type = string_to_gun[gun.first.as<std::string>()];
            int x = gun.second["x"].as<int>();
            int y = gun.second["y"].as<int>();
            int facing_right_x_offset = gun.second["facing_right_x_offset"].as<int>();
            int facing_left_x_offset = gun.second["facing_left_x_offset"].as<int>();
            int y_offset = gun.second["y_offset"].as<int>();
            int width = gun.second["width"].as<int>();
            int height = gun.second["height"].as<int>();

            guns[gun_type] = new Animation(*sprite, {Rect(x, y, width, height)}, 1, false);
            SpriteInfo offset = {width, height, facing_right_x_offset, facing_left_x_offset, y_offset};
            sprites_info[gun_type] = offset;
        }

        current_gun = None;
        curr_animation = guns[None];
    }

    void update() {
        curr_animation->update();
    }

    void update_from_snapshot(const Duck& duck) {
        x = duck.x;
        y = duck.y;

        facing_right = duck.facing_right;
        facing_up = duck.facing_up;

        curr_animation = guns[duck.gun];
        current_gun = duck.gun;
    }

    void render(SDL2pp::Renderer& renderer, Camera& camera) {
        if (current_gun == None) {
            return;
        }

        SpriteInfo info = sprites_info[current_gun];

        int x_offset = facing_right ? info.facing_right_x_offset : info.facing_left_x_offset;
        Rect dst = SDL2pp::Rect(x - x_offset, y - info.y_offset, info.width, info.height);

        SDL_RendererFlip flip = facing_right ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL ;

        float angle = facing_up ? facing_right ? 290 : 70 : 0;

        curr_animation->render(
            renderer,
            camera,
            dst,
            flip,
            angle);
    }

    ~RenderableEquippedGun() {
        for (auto& gun : guns) {
            delete gun.second;
        }
    }
};

#endif
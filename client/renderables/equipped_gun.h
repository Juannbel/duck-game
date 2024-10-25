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

    static std::unordered_map<GunType, std::string> gun_to_string;
    void load_gun_animation(const GunType& gun);

public:
    RenderableEquippedGun();

    void update();

    void update_from_snapshot(const Duck& duck);

    void render(SDL2pp::Renderer& renderer, Camera& camera);

    ~RenderableEquippedGun();
};

#endif

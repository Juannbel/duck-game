#ifndef RENDERABLE_DUCK_H
#define RENDERABLE_DUCK_H

#include <string>
#include <unordered_map>

#include <SDL2pp/SDL2pp.hh>
#include <SDL_render.h>

#include "SDL2pp/Point.hh"
#include "client/renderables/equipped_gun.h"
#include "common/snapshot.h"

#include "animation.h"

class RenderableDuck {
private:
    void load_animations(SDL2pp::Texture* sprite, const std::string& config_path);

    Animation* curr_animation;
    RenderableEquippedGun gun;

    std::unordered_map<std::string, Animation*> animations;

    SDL2pp::Point position;

    bool is_facing_right;
    bool is_alive;

public:
    RenderableDuck(SDL2pp::Texture* sprite, const std::string& config_path,
                   SDL2pp::Texture* guns_sprite, const std::string& guns_config);


    void update();

    void render(SDL2pp::Renderer& renderer, Camera& camera);

    void skip_frames(uint8_t frames);

    void update_from_snapshot(const Duck& duck);

    bool is_dead();

    SDL2pp::Rect get_bounding_box();

    ~RenderableDuck();
};

#endif

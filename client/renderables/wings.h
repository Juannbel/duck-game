#ifndef RENDERABLE_WINGS_H
#define RENDERABLE_WINGS_H

#include <string>
#include <unordered_map>

#include <SDL_render.h>
#include <yaml-cpp/yaml.h>

#include "common/snapshot.h"

#include "animation.h"
class RenderableWings {
private:
    uint8_t duck_id;
    Animation* curr_animation;

    std::unordered_map<std::string, Animation*> animations;

    SDL2pp::Point position;

    bool facing_right;

    void load_animation(const std::string& name);

public:
    explicit RenderableWings(uint8_t duck_id);

    void update(const Duck& duck);

    void render(SDL2pp::Renderer& renderer, Camera& camera);

    void skip_frames(uint8_t frames);

    ~RenderableWings();
};

#endif

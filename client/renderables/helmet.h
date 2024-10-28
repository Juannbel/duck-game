#ifndef RENDERABLE_HELMET_H
#define RENDERABLE_HELMET_H

#include <unordered_map>
#include <SDL2pp/SDL2pp.hh>
#include "client/renderables/animation.h"

class RenderableHelmet {
private:
    Animation* curr_animation;

    std::unordered_map<std::string, Animation*> animations;

    SDL2pp::Point position;

    bool facing_right;

public:
    RenderableHelmet();

    void update(const Duck& duck);

    void render(SDL2pp::Renderer& renderer, Camera& camera);

    void skip_frames(uint8_t frames);

    ~RenderableHelmet();
};

#endif

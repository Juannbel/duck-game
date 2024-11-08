#ifndef RENDERABLE_FEATHERS_H
#define RENDERABLE_FEATHERS_H

#include "client/camera.h"

#include "animation.h"

class RenderableFeathers {
private:
    Animation animation;
    SDL2pp::Point position;
    bool never_damaged;

public:
    explicit RenderableFeathers(uint8_t duck_id);

    void render(SDL2pp::Renderer& renderer, Camera& camera);

    void update(const Duck& duck);

    void skip_frames(uint8_t frames);
};

#endif

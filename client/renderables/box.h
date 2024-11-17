#ifndef RENDERABLE_BOX_H
#define RENDERABLE_BOX_H

#include <map>

#include <SDL2pp/SDL2pp.hh>

#include "common/snapshot.h"

#include "animation.h"

class RenderableBox {
private:
    uint32_t id;
    Animation* curr_animation;

    std::map<BoxStatus, Animation*> animations;

    SDL2pp::Point position;

public:
    explicit RenderableBox(uint32_t id);

    void update(const Box& box_snapshot);

    void render(SDL2pp::Renderer& renderer, Camera& camera);

    uint32_t get_id();

    ~RenderableBox();
};

#endif

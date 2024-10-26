#ifndef RENDERABLE_COLLECTABLE_H
#define RENDERABLE_COLLECTABLE_H

#include <string>
#include "SDL2pp/Point.hh"
#include "SDL2pp/Texture.hh"
#include "client/renderables/animation.h"
#include "common/snapshot.h"

class RenderableCollectable {
    private:
    uint32_t id;
    Animation animation;

    SDL2pp::Point position;
    static std::unordered_map<GunType, std::string> collectable_to_string;

    public:
    RenderableCollectable(uint32_t id, GunType type);

    void update(const Gun& snapshot);

    void render(SDL2pp::Renderer& renderer, Camera& camera);

    uint32_t get_id();
};

#endif

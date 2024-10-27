#ifndef RENDERABLE_BULLET_H
#define RENDERABLE_BULLET_H

#include <cstdint>
#include "client/camera.h"
#include "common/snapshot.h"
#include <SDL2pp/SDL2pp.hh>

class RenderableBullet {
private:
    uint32_t id;
    // Animation animation;
    GunType type;

    SDL2pp::Point position;

    public:
    RenderableBullet(uint32_t id, GunType type);

    void update(const Bullet& snapshot);

    void render(SDL2pp::Renderer& renderer, Camera& camera);

    uint32_t get_id();
};

#endif

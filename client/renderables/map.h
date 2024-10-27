#ifndef RENDERABLE_MAP_H
#define RENDERABLE_MAP_H

#include <vector>

#include <SDL2pp/SDL2pp.hh>

#include "common/map_dto.h"
#include "client/camera.h"

#include "block.h"

class RenderableMap {
private:
    SDL2pp::Texture* background_texture;

    // representación del mapa
    std::vector<RenderableBlock> map;

public:
    RenderableMap(const Map& map_dto, uint8_t id);

    void render(SDL2pp::Renderer& renderer, Camera& camera);
};

#endif

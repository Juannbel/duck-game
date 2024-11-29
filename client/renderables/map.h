#ifndef RENDERABLE_MAP_H
#define RENDERABLE_MAP_H

#include <memory>
#include <vector>

#include <SDL2pp/SDL2pp.hh>

#include "client/camera.h"
#include "client/textures_provider.h"
#include "common/map_dto.h"

#include "block.h"

class RenderableMap {
private:
    std::shared_ptr<SDL2pp::Texture> background_texture;
    TexturesProvider& textures_provider;

    // representación del mapa
    std::vector<RenderableBlock> map;

public:
    explicit RenderableMap(SDL2pp::Renderer& renderer);

    void render(SDL2pp::Renderer& renderer, Camera& camera);

    void render_first_round(SDL2pp::Renderer& renderer, Camera& camera);

    void update(const MapDto& new_map_dto);
};

#endif

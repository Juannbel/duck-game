#ifndef RENDERABLE_MAP_H
#define RENDERABLE_MAP_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <SDL2pp/SDL2pp.hh>

#include "client/camera.h"
#include "common/map_dto.h"

#include "block.h"

class RenderableMap {
private:
    std::shared_ptr<SDL2pp::Texture> background_texture;

    // representación del mapa
    std::vector<RenderableBlock> map;

    static std::unordered_map<BlockType, std::string> block_to_string;

public:
    explicit RenderableMap(const MapDto& map_dto);

    void render(SDL2pp::Renderer& renderer, Camera& camera);

    void update(const MapDto& new_map_dto);
};

#endif

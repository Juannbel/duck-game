#ifndef RENDERABLE_MAP_H
#define RENDERABLE_MAP_H

#include <memory>
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
    RenderableMap(const Map& map_dto, uint8_t id);

    void render(SDL2pp::Renderer& renderer, Camera& camera);

    void update(const Map& new_map_dto, uint8_t theme);
};

#endif

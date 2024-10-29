#include "map.h"

#include <SDL2pp/SDL2pp.hh>

#include "client/textures_provider.h"
#include "common/shared_constants.h"

RenderableMap::RenderableMap(const Map& map_dto, uint8_t theme) { update(map_dto, theme); }

void RenderableMap::render(SDL2pp::Renderer& renderer, Camera& camera) {
    SDL2pp::Rect background_rect(-MAP_WIDTH_PIXELS / 2, -MAP_HEIGHT_PIXELS / 2,
                                 MAP_WIDTH_PIXELS * 2, MAP_HEIGHT_PIXELS * 2);
    camera.transform_rect(background_rect);
    renderer.Copy(*background_texture, SDL2pp::NullOpt, background_rect);

    renderer.SetDrawBlendMode(SDL_BLENDMODE_BLEND);
    renderer.SetDrawColor(0, 0, 0, 50);
    renderer.FillRect(background_rect);
    renderer.SetDrawBlendMode();

    for (auto& block: map) {
        block.render(renderer, camera);
    }
}

void RenderableMap::update(const Map& new_map_dto, uint8_t theme) {
    map.clear();
    background_texture = TexturesProvider::get_texture("background_" + std::to_string(theme));

    std::shared_ptr<SDL2pp::Texture> blocks_texture(
            TexturesProvider::get_texture("blocks_" + std::to_string(theme)));
    for (int i = 0; i < MAP_HEIGHT_BLOCKS; i++) {
        for (int j = 0; j < MAP_WIDTH_BLOCKS; j++) {
            SDL2pp::Rect src_rect(0, 0, BLOCK_SIZE, BLOCK_SIZE);
            SDL2pp::Rect dst_rect(j * BLOCK_SIZE, i * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);

            switch (new_map_dto.blocks[i][j]) {
                case Empty:
                    continue;
                case Floor:
                    break;
                case Wall:
                    src_rect.x = BLOCK_SIZE;
                    break;
                case Core:
                    src_rect.x = BLOCK_SIZE * 2;
                    break;
            }

            map.push_back(RenderableBlock(src_rect, dst_rect, blocks_texture));
        }
    }
}

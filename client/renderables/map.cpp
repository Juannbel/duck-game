#include "map.h"

#include "common/shared_constants.h"

RenderableMap::RenderableMap(const Map& map_dto, SDL2pp::Texture* blocks_texture,
                SDL2pp::Texture* background_texture):
        background_texture(background_texture) {
    for (int i = 0; i < MAP_HEIGHT_BLOCKS; i++) {
        for (int j = 0; j < MAP_WIDTH_BLOCKS; j++) {
            SDL2pp::Rect src_rect(0, 0, BLOCK_SIZE, BLOCK_SIZE);
            SDL2pp::Rect dst_rect(j * BLOCK_SIZE, i * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);

            switch (map_dto.blocks[i][j]) {
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
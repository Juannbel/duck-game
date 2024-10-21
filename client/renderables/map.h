#ifndef RENDERABLE_MAP_H
#define RENDERABLE_MAP_H

#include "client/camera.h"
#include <SDL2pp/SDL2pp.hh>
#include "../../common/shared_constants.h"

#include <vector>

enum BlockType {
    Empty,
    Floor,
    Wall,
    Core
};

struct Map {
    BlockType blocks[MAP_HEIGHT_BLOCKS][MAP_WIDTH_BLOCKS];
};

class RenderableBlock {
private:
    SDL2pp::Rect src_rect;
    SDL2pp::Rect dst_rect;
    SDL2pp::Texture *texture;

public:
    RenderableBlock(SDL2pp::Rect src_rect, SDL2pp::Rect dst_rect, SDL2pp::Texture *texture) : src_rect(src_rect), dst_rect(dst_rect), texture(texture) {}

    void render(SDL2pp::Renderer &renderer, Camera &camera) {
        SDL2pp::Rect d_rect = dst_rect;
        if (!camera.is_rect_visible(d_rect)) {
            return;
        }
        camera.transform_rect(d_rect);
        renderer.Copy(*texture, src_rect, d_rect);
    }
};

class RenderableMap {
private:
    SDL2pp::Texture *background_texture;

    int block_size = BLOCK_SIZE;

    // representación del mapa
    std::vector<RenderableBlock> map;

public:

    RenderableMap(const Map& map_dto, SDL2pp::Texture *blocks_texture, SDL2pp::Texture *background_texture) : background_texture(background_texture) {
        for (int i = 0; i < MAP_HEIGHT_BLOCKS; i++) {
            for (int j = 0; j < MAP_WIDTH_BLOCKS; j++) {
                SDL2pp::Rect src_rect;
                SDL2pp::Rect dst_rect;

                switch (map_dto.blocks[i][j]) {
                    case Empty:
                        continue;
                    case Floor:
                        src_rect = SDL2pp::Rect(0, 0, block_size, block_size);
                        break;
                    case Wall:
                        src_rect = SDL2pp::Rect(block_size, 0, block_size, block_size);
                        break;
                    case Core:
                        src_rect = SDL2pp::Rect(block_size * 2, 0, block_size, block_size);
                        break;
                }

                dst_rect = SDL2pp::Rect(j * block_size, i * block_size, block_size, block_size);

                map.push_back(RenderableBlock(src_rect, dst_rect, blocks_texture));
            }
        }
    }

    void render(SDL2pp::Renderer &renderer, Camera &camera) {
        SDL2pp::Rect background_rect(
            -MAP_WIDTH_PIXELS/2, -MAP_HEIGHT_PIXELS/2,
            MAP_WIDTH_PIXELS * 2, MAP_HEIGHT_PIXELS * 2);
        camera.transform_rect(background_rect);
        renderer.Copy(*background_texture, NullOpt, background_rect);

        renderer.SetDrawBlendMode(SDL_BLENDMODE_BLEND);
        renderer.SetDrawColor(0,0,0, 50);
        renderer.FillRect(background_rect);
        renderer.SetDrawBlendMode();

        for (auto& block : map) {
            block.render(renderer, camera);
        }
    }
};

#endif

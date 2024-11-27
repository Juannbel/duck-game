#ifndef RENDERABLE_BLOCK_H
#define RENDERABLE_BLOCK_H

#include <memory>

#include <SDL2pp/SDL2pp.hh>
#include <SDL_blendmode.h>

#include "client/camera.h"
#include "common/config.h"

static Config& conf = Config::get_instance();

const static uint8_t NON_SOLID_OPACITY = conf.get_non_solid_opacity();
class RenderableBlock {
private:
    SDL2pp::Rect src_rect;
    SDL2pp::Rect dst_rect;
    std::shared_ptr<SDL2pp::Texture> texture;
    bool solid;

public:
    RenderableBlock(SDL2pp::Rect src_rect, SDL2pp::Rect dst_rect,
                    std::shared_ptr<SDL2pp::Texture> texture, bool solid):
            src_rect(src_rect), dst_rect(dst_rect), texture(texture), solid(solid) {
            }

    void render(SDL2pp::Renderer& renderer, Camera& camera) {
        if (!camera.is_rect_visible(dst_rect))
            return;
        SDL2pp::Rect d_rect = dst_rect;

        camera.transform_rect(d_rect);

        if (!solid) {
            texture->SetAlphaMod(NON_SOLID_OPACITY);
        }

        renderer.Copy(*texture, src_rect, d_rect);

        if (!solid) {
            texture->SetAlphaMod();
        }

    }
};

#endif

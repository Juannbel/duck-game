#ifndef RENDERABLE_BLOCK_H
#define RENDERABLE_BLOCK_H

#include <SDL2pp/SDL2pp.hh>
#include "client/camera.h"

class RenderableBlock {
private:
    SDL2pp::Rect src_rect;
    SDL2pp::Rect dst_rect;
    SDL2pp::Texture* texture;

public:
    RenderableBlock(SDL2pp::Rect src_rect, SDL2pp::Rect dst_rect, SDL2pp::Texture* texture):
            src_rect(src_rect), dst_rect(dst_rect), texture(texture) {}

    void render(SDL2pp::Renderer& renderer, Camera& camera) {
        SDL2pp::Rect d_rect = dst_rect;
        if (!camera.is_rect_visible(d_rect)) {
            return;
        }
        camera.transform_rect(d_rect);
        renderer.Copy(*texture, src_rect, d_rect);
    }
};

#endif
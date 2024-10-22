#ifndef ANIMATION_H
#define ANIMATION_H

#include <cstddef>
#include <cstdint>
#include <vector>

#include <SDL2pp/SDL2pp.hh>
#include <SDL_render.h>

#include "client/camera.h"

#define FRAME_RATE 1000.0f / 30.0f

struct FrameData {
    SDL2pp::Rect rect;
    int x_offset_right;
    int x_offset_left;
    int y_offset;
};

class Animation {
protected:
    SDL2pp::Texture& texture;

    size_t currentFrame;
    // Cantidad de iteraciones que pasaron desde el ultimo cambio de frame
    uint8_t iterations_since_change;
    // Cantidad de iteraciones que deben pasar para cambiar de frame
    uint8_t iterations_per_frame;
    std::vector<FrameData> frames;

    void advance_frame();

    bool loops;

public:
    // Recibe la textura y el path del archivo de configuracion
    Animation(SDL2pp::Texture& texture, std::vector<FrameData> frames,
              uint8_t iterations_per_frame, bool loops);

    void update();

    void render(SDL2pp::Renderer& renderer, Camera& camera, SDL2pp::Point& dest,
                bool facing_right = true, float angle = 0.0f);

    void skip_frames(uint8_t frames_to_skip);

    void restart();
};

#endif

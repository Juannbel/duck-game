#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL2pp/SDL2pp.hh>
#include <SDL_render.h>
#include <cstdint>
#include <vector>

#define FRAME_RATE 1000.0f/30.0f

using namespace SDL2pp;

class Animation {
    public:
    // Recibe la textura y el path del archivo de configuracion
    Animation(SDL2pp::Texture &texture, std::vector<Rect> frames, uint8_t iterations_per_frame);
    ~Animation();
    void update();
    void render(Renderer &renderer, const Rect dest, SDL_RendererFlip &flipType);
    void skip_frames(uint8_t frames);
    void restart();

   private:
    void advance_frame();

    SDL2pp::Texture &texture;

    int currentFrame;

    // Cantidad de iteraciones que pasaron desde el ultimo cambio de frame
    uint8_t iterations_since_change;

    // Cantidad de iteraciones que deben pasar para cambiar de frame
    uint8_t iterations_per_frame;

    std::vector<Rect> frames;
};

#endif
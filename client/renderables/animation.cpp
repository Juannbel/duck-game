#include <SDL2pp/SDL2pp.hh>

#include <cassert>

#include "animation.h"

#include <vector>

Animation::Animation(SDL2pp::Texture &texture, std::vector<Rect> frames, uint8_t iterations_per_frame) : texture(texture), currentFrame(0), iterations_since_change(0), iterations_per_frame(iterations_per_frame), frames(frames) {
}

void Animation::update() {
    if (iterations_since_change == iterations_per_frame) {
        this->advance_frame();
        iterations_since_change = 0;
    } else {
        iterations_since_change++;
    }
}

void Animation::render(SDL2pp::Renderer &renderer, const SDL2pp::Rect dst, SDL_RendererFlip &flipType) {
    renderer.Copy(
            texture,
            frames[currentFrame],
            dst,
            flipType
        );
}

void Animation::advance_frame() {
    this->currentFrame += 1;
    this->currentFrame = this->currentFrame % this->frames.size(); 
}

void Animation::skip_frames(uint8_t frames) {
    this->currentFrame += frames;
    this->currentFrame = this->currentFrame % this->frames.size();
}

void Animation::restart() {
    this->currentFrame = 0;
}
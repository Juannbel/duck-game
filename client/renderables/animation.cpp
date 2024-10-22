#include "animation.h"

#include <cassert>
#include <vector>

#include <SDL2pp/SDL2pp.hh>

#include "SDL2pp/Rect.hh"

using SDL2pp::NullOpt;
using SDL2pp::Rect;

Animation::Animation(SDL2pp::Texture& texture, std::vector<Rect> frames,
                     uint8_t iterations_per_frame, bool loops):
        texture(texture),
        currentFrame(0),
        iterations_since_change(0),
        iterations_per_frame(iterations_per_frame),
        frames(frames),
        loops(loops) {}

void Animation::update() {
    if (iterations_since_change == iterations_per_frame) {
        this->advance_frame();
        iterations_since_change = 0;
    } else {
        iterations_since_change++;
    }
}

void Animation::render(SDL2pp::Renderer& renderer, Camera& camera, SDL2pp::Rect& dst,
                       SDL_RendererFlip& flipType, float angle) {
    if (!camera.is_rect_visible(dst)) {
        return;
    }

    camera.transform_rect(dst);
    renderer.Copy(texture, frames[currentFrame], dst, angle, NullOpt, flipType);
}

void Animation::advance_frame() {
    if (currentFrame == frames.size() - 1 && !loops) {
        return;
    }
    currentFrame += 1;
    currentFrame = currentFrame % frames.size();
}

void Animation::skip_frames(uint8_t frames_to_skip) {
    currentFrame += frames_to_skip;
    if (!loops && currentFrame > frames.size() - 1) {
        currentFrame = frames.size() - 1;
    }
    currentFrame = currentFrame % frames.size();
}

void Animation::restart() { currentFrame = 0; }

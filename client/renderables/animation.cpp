#include "animation.h"

#include <cassert>
#include <vector>

#include <SDL2pp/SDL2pp.hh>

#include "SDL2pp/Optional.hh"
#include "SDL2pp/Point.hh"
#include "SDL2pp/Rect.hh"

Animation::Animation(SDL2pp::Texture& texture, const AnimationData& data) :
        texture(texture),
        current_frame(0),
        iterations_since_change(0),
        data(data) {}

void Animation::update() {
    if (iterations_since_change == data.iter_per_frame) {
        this->advance_frame();
        iterations_since_change = 0;
    } else {
        iterations_since_change++;
    }
}

void Animation::render(SDL2pp::Renderer& renderer, Camera& camera, const SDL2pp::Point& dest,
                       bool facing_right, float angle) {
    const FrameData& frame_data = data.frames[current_frame];
    int x_offset = facing_right ? frame_data.x_offset_right : frame_data.x_offset_left;
    SDL_RendererFlip flipType = facing_right ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;

    SDL2pp::Rect dest_rect = SDL2pp::Rect(dest.x - x_offset, dest.y - frame_data.y_offset,
                                          frame_data.rect.w, frame_data.rect.h);

    if (!camera.is_rect_visible(dest_rect)) {
        return;
    }

    camera.transform_rect(dest_rect);

    renderer.Copy(texture, frame_data.rect, dest_rect, angle, SDL2pp::NullOpt, flipType);
}

void Animation::advance_frame() {
    if (current_frame == data.frames.size() - 1 && !data.loops) {
        return;
    }
    current_frame += 1;
    current_frame = current_frame % data.frames.size();
}

void Animation::skip_frames(uint8_t frames_to_skip) {
    current_frame += frames_to_skip;
    if (!data.loops && current_frame > data.frames.size() - 1) {
        current_frame = data.frames.size() - 1;
    }
    current_frame = current_frame % data.frames.size();
}

void Animation::restart() { current_frame = 0; }

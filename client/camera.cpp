#include "camera.h"

#include <algorithm>
#include <cassert>
#include <limits>

#include "common/snapshot.h"
#include "common/shared_constants.h"

#define CAMERA_LERP_FACTOR 0.1f  // que tan rápido se mueve la cámara (entre 0.0 y 1.0)
#define CAMERA_DEAD_ZONE 20.0f   // distancia mínima para mover la cámara
#define PADDING 50

#define MARGIN_W (MAP_WIDTH_PIXELS / 2)
#define MARGIN_H (MAP_HEIGHT_PIXELS / 2)

using SDL2pp::Rect;
using SDL2pp::Renderer;

Camera::Camera(Renderer& renderer):
        renderer(renderer),
        current_rect(-MARGIN_W, -MARGIN_H, MAP_WIDTH_PIXELS + 2 * MARGIN_W, MAP_HEIGHT_PIXELS + 2 * MARGIN_H),
        target_rect(-MARGIN_W, -MARGIN_H, MAP_WIDTH_PIXELS + 2 * MARGIN_W, MAP_HEIGHT_PIXELS + 2 * MARGIN_H),
        scale_x((float)renderer.GetOutputWidth() / current_rect.w),
        scale_y((float)renderer.GetOutputHeight() / current_rect.h),
        scale(std::min(scale_x, scale_y)) {
    assert(CAMERA_LERP_FACTOR >= 0.0f && CAMERA_LERP_FACTOR <= 1.0f);
}

void Camera::update(const Snapshot& snapshot) {
    update_target(snapshot);

    float dx = target_rect.x - current_rect.x;
    float dy = target_rect.y - current_rect.y;
    float dw = target_rect.w - current_rect.w;
    float dh = target_rect.h - current_rect.h;

    if (std::abs(dx) < CAMERA_DEAD_ZONE && std::abs(dy) < CAMERA_DEAD_ZONE &&
        std::abs(dw) < CAMERA_DEAD_ZONE && std::abs(dh) < CAMERA_DEAD_ZONE) {
        return;
    }

    current_rect.x += dx * CAMERA_LERP_FACTOR;
    current_rect.y += dy * CAMERA_LERP_FACTOR;
    current_rect.w += dw * CAMERA_LERP_FACTOR;
    current_rect.h += dh * CAMERA_LERP_FACTOR;

    scale_x = (float)renderer.GetOutputWidth() / current_rect.w;
    scale_y = (float)renderer.GetOutputHeight() / current_rect.h;
    scale = std::min(scale_x, scale_y);
}

void Camera::transform_rect(Rect& world_rect) {
    world_rect.x = (world_rect.x - current_rect.x) * scale;
    world_rect.y = (world_rect.y - current_rect.y) * scale;
    world_rect.w = world_rect.w * scale;
    world_rect.h = world_rect.h * scale;
}

bool Camera::is_rect_visible(const Rect& world_rect) { return current_rect.Intersects(world_rect); }

void Camera::adjust_aspect_ratio(Rect& target) {
    float window_aspect_ratio = (float)renderer.GetOutputWidth() / renderer.GetOutputHeight();
    float target_aspect_ratio = (float)target.w / target.h;

    if (target_aspect_ratio > window_aspect_ratio) {
        target.h = target.w / window_aspect_ratio;
    } else {
        target.w = target.h * window_aspect_ratio;
    }

    if (target.x < -MARGIN_W) {
        target.x = -MARGIN_W;
    }

    if (target.y < -MARGIN_H) {
        target.y = -MARGIN_H;
    }

    if (target.x + target.w > MAP_WIDTH_PIXELS + MARGIN_W) {
        target.x = MAP_WIDTH_PIXELS + MARGIN_W - target.w;
    }

    if (target.y + target.h > MAP_HEIGHT_PIXELS + MARGIN_H) {
        target.y = MAP_HEIGHT_PIXELS + MARGIN_H - target.h;
    }
}

void Camera::update_target(const Snapshot& snapshot) {
    int16_t left = std::numeric_limits<int16_t>::max();
    int16_t right = std::numeric_limits<int16_t>::min();
    int16_t top = std::numeric_limits<int16_t>::max();
    int16_t bottom = std::numeric_limits<int16_t>::min();

    for (auto& duck: snapshot.ducks) {
        if (duck.is_dead) {
            continue;
        }

        left = std::min(left, duck.x);
        right = std::max(right, (int16_t)(duck.x + DUCK_HITBOX_WIDTH));
        top = std::min(top, duck.y);
        bottom = std::max(bottom, (int16_t)(duck.y + DUCK_HITBOX_HEIGHT));
    }

    target_rect.x = left - PADDING;
    target_rect.y = top - PADDING;
    target_rect.w = right - left + 2 * PADDING;
    target_rect.h = bottom - top + 2 * PADDING;

    adjust_aspect_ratio(target_rect);
}

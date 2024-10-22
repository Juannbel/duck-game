#include "camera.h"

#include <algorithm>
#include <cassert>

using SDL2pp::Rect;
using SDL2pp::Renderer;

Camera::Camera(Renderer& renderer):
        renderer(renderer),
        current_rect(0, 0, renderer.GetOutputWidth(), renderer.GetOutputHeight()),
        target_rect(0, 0, renderer.GetOutputWidth(), renderer.GetOutputHeight()),
        scale_x(1.0f),
        scale_y(1.0f),
        scale(1.0f) {
    assert(CAMERA_LERP_FACTOR >= 0.0f && CAMERA_LERP_FACTOR <= 1.0f);
}

void Camera::set_target(Rect& target) {
    target.x -= PADDING;
    target.y -= PADDING;
    target.w += 2 * PADDING;
    target.h += 2 * PADDING;
    target_rect = adjust_aspect_ratio(target);
}

void Camera::update() {
    float dx = target_rect.x - current_rect.x;
    float dy = target_rect.y - current_rect.y;
    float dw = target_rect.w - current_rect.w;
    float dh = target_rect.h - current_rect.h;

    // No actualizar si estamos dentro de la zona muerta
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

void Camera::transform_rect(Rect& worldRect) {
    worldRect.x = (worldRect.x - current_rect.x) * scale;
    worldRect.y = (worldRect.y - current_rect.y) * scale;
    worldRect.w = worldRect.w * scale;
    worldRect.h = worldRect.h * scale;
}

bool Camera::is_rect_visible(const Rect& worldRect) { return current_rect.Intersects(worldRect); }

Rect Camera::adjust_aspect_ratio(const Rect& target) {
    float windowAspectRatio = (float)renderer.GetOutputWidth() / renderer.GetOutputHeight();
    float targetAspectRatio = (float)target.w / target.h;

    int width = target.w;
    int height = target.h;

    if (targetAspectRatio > windowAspectRatio) {
        height = width / windowAspectRatio;
    } else {
        width = height * windowAspectRatio;
    }

    int centerX = target.x + target.w / 2;
    int centerY = target.y + target.h / 2;

    return Rect(centerX - width / 2, centerY - height / 2, width, height);
}

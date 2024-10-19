#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2pp/SDL2pp.hh>

#define CAMERA_LERP_FACTOR 0.1f // que tan rapido se mueve la camara
#define CAMERA_DEAD_ZONE 10.0f  // si la camara esta a menos de esta distancia de su objetivo no se mueve

class Camera {
public:
    Camera(int width, int height) 
        : currentRect{0, 0, width, height}, targetRect{0, 0, width, height} {}

    void setTarget(const SDL2pp::Rect& target) {
        targetRect = target;
    }

    void update() {
        float dx = targetRect.x - currentRect.x;
        float dy = targetRect.y - currentRect.y;
        float dw = targetRect.w - currentRect.w;
        float dh = targetRect.h - currentRect.h;

        if (std::abs(dx) < CAMERA_DEAD_ZONE && std::abs(dy) < CAMERA_DEAD_ZONE &&
            std::abs(dw) < CAMERA_DEAD_ZONE && std::abs(dh) < CAMERA_DEAD_ZONE) {
            return;  // No actualizar si estamos dentro de la zona muerta
        }

        currentRect.x += dx * CAMERA_LERP_FACTOR;
        currentRect.y += dy * CAMERA_LERP_FACTOR;
        currentRect.w += dw * CAMERA_LERP_FACTOR;
        currentRect.h += dh * CAMERA_LERP_FACTOR;

        if (currentRect.x < 0) currentRect.x = 0;
        if (currentRect.y < 0) currentRect.y = 0;
        if (currentRect.x + currentRect.w > 1200) currentRect.x = 1200 - currentRect.w;
        if (currentRect.y + currentRect.h > 800) currentRect.y = 800 - currentRect.h;
    }

    SDL2pp::Rect getCurrentRect() const {
        return SDL2pp::Rect(
            static_cast<int>(std::round(currentRect.x)),
            static_cast<int>(std::round(currentRect.y)),
            static_cast<int>(std::round(currentRect.w)),
            static_cast<int>(std::round(currentRect.h))
        );
    }

private:
    SDL2pp::Rect currentRect;
    SDL2pp::Rect targetRect;
};

#endif
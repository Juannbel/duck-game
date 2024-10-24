#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2pp/SDL2pp.hh>

#include "common/snapshot.h"

#define CAMERA_LERP_FACTOR 0.1f  // que tan rápido se mueve la cámara (entre 0.0 y 1.0)
#define CAMERA_DEAD_ZONE 20.0f   // distancia mínima para mover la cámara
#define PADDING 40

class Camera {

private:
    SDL2pp::Renderer& renderer;

    SDL2pp::Rect current_rect;
    SDL2pp::Rect target_rect;

    float scale_x, scale_y, scale;

    // Ajusta el aspect ratio del target para que se vea bien en la ventana
    void adjust_aspect_ratio(SDL2pp::Rect& target);

    SDL2pp::Rect get_bounding_rect(const Snapshot& snapshot);

    void update_target(const Snapshot& snapshot);

public:
    explicit Camera(SDL2pp::Renderer& renderer);

    void update(const Snapshot& snapshot);

    // Transorma un rectángulo con las coordenadas y la escala del mundo
    // a las coordenadas de la cámara
    void transform_rect(SDL2pp::Rect& world_rect);

    // Devuelve true si el rectángulo es visible en la cámara
    bool is_rect_visible(const SDL2pp::Rect& world_rect);
};

#endif

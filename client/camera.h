#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2pp/SDL2pp.hh>

#include "common/snapshot.h"

class Camera {

private:
    SDL2pp::Renderer& renderer;

    SDL2pp::Rect current_rect;
    SDL2pp::Rect target_rect;

    float zoom_x, zoom_y, zoom;

    // Ajusta el aspect ratio del target para que se vea bien en la ventana
    void adjust_aspect_ratio(SDL2pp::Rect& target);

    SDL2pp::Rect get_bounding_rect(const Snapshot& snapshot);

    void update_target(const Snapshot& snapshot);

public:
    explicit Camera(SDL2pp::Renderer& renderer);

    const SDL2pp::Rect& get_current_rect();

    // Hace el update de la camara, mueve el target basado en la snapshot si move_target es true,
    // mueve lo que se ve actualmente hacia el target, y se ajusta a la ventana
    void update(const Snapshot& snapshot, bool move_target);

    // Transorma un rectángulo con las coordenadas y la escala del mundo
    // a las coordenadas de la cámara
    void transform_rect(SDL2pp::Rect& world_rect);

    // Devuelve true si el rectángulo es visible en la cámara
    bool is_rect_visible(const SDL2pp::Rect& world_rect);
};

#endif

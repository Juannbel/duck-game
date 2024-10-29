#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include <cstdint>
#include <memory>
#include <SDL2pp/SDL2pp.hh>

#include "client/camera.h"
#include "client/renderables/animation.h"
#include "client/renderables/map.h"
#include "common/blocking_queue.h"
#include "common/snapshot.h"

class ScreenManager {
private:
    uint8_t duck_id;
    SDL2pp::Renderer& renderer;
    SDL2pp::SDLTTF sdl_ttf;
    SDL2pp::Font primary_font;

    void render_duck_stat(Duck& duck, SDL2pp::Rect rect, std::shared_ptr<SDL2pp::Texture> ducks_texture, AnimationData& animation_data);

public:
    ScreenManager(SDL2pp::Renderer& renderer, uint8_t duck_id);

    bool waiting_screen(Queue<Snapshot> &snapshot_q, Snapshot &last_snapshot);

    /*
    * Pre: last_snapshot es valida, para poder mostrar las estadisticas
    * Devuelve true si se sigue jugando, false si no
    * Puede devolver false por dos razones:
    *       - el jugador quiere salir
    *       - el juego terminó
    * En el segundo caso, muestra tambien la pantalla de fin de juego
    */
    bool between_rounds_screen(Queue<Snapshot> &snapshot_q, Snapshot &last_snapshot, RenderableMap& map, Camera& camera);
};

#endif

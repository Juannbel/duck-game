#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include <cstdint>
#include <memory>
#include <utility>

#include <SDL2pp/SDL2pp.hh>

#include "client/camera.h"
#include "client/renderables/animation.h"
#include "client/renderables/map.h"
#include "common/blocking_queue.h"
#include "common/snapshot.h"

class ScreenManager {
private:
    std::pair<uint8_t, uint8_t>& duck_ids;
    SDL2pp::Renderer& renderer;
    SDL2pp::SDLTTF sdl_ttf;
    SDL2pp::Font primary_font;

    void render_duck_stat(const Duck& duck, SDL2pp::Rect rect,
                          std::shared_ptr<SDL2pp::Texture> ducks_texture,
                          AnimationData& animation_data);

    bool stats_screen(Queue<Snapshot>& snapshot_q, Snapshot& last_snapshot,
                                              RenderableMap& map, Camera& camera);

    bool end_game_screen(Snapshot& last_snapshot, RenderableMap& map, Camera& camera);

public:
    ScreenManager(SDL2pp::Renderer& renderer, std::pair<uint8_t, uint8_t>& duck_ids);

    bool waiting_screen(Queue<Snapshot>& snapshot_q, Snapshot& last_snapshot);

    bool initial_screen(Queue<Snapshot>& snapshot_q, Snapshot& last_snapshot, RenderableMap& map,
                        Camera& camera);

    bool between_rounds_screen(Queue<Snapshot>& snapshot_q, Snapshot& last_snapshot,
                               RenderableMap& map, Camera& camera);
};

#endif

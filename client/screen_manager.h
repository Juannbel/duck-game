#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include <cstdint>
#include <memory>
#include <utility>

#include <SDL2pp/SDL2pp.hh>

#include "client/camera.h"
#include "client/renderables/animation.h"
#include "client/renderables/map.h"
#include "client/sound_manager.h"
#include "common/blocking_queue.h"
#include "common/snapshot.h"

class ScreenManager {
private:
    SDL2pp::Window& window;
    SoundManager& sound_manager;
    SDL2pp::Renderer& renderer;
    std::pair<uint8_t, uint8_t>& duck_ids;
    SDL2pp::SDLTTF sdl_ttf;
    SDL2pp::Font primary_font;
    RenderableMap& map;
    Camera& camera;
    bool& play_again;
    bool owner_started_game;

    void render_duck_stat(const Duck& duck, SDL2pp::Rect rect,
                          std::shared_ptr<SDL2pp::Texture> ducks_texture,
                          AnimationData& animation_data);

    bool stats_screen(Queue<Snapshot>& snapshot_q, Snapshot& last_snapshot);

    bool end_game_no_winner_screen();

    bool end_game_screen(Snapshot& last_snapshot);


public:
    ScreenManager(SDL2pp::Window& window, SoundManager& sound_manager, SDL2pp::Renderer& renderer,
                  Camera& camera, RenderableMap& map, std::pair<uint8_t, uint8_t>& duck_ids,
                  bool& play_again);

    bool between_rounds_screen(Queue<Snapshot>& snapshot_q, Snapshot& last_snapshot);

    void show_lobby_text(Snapshot& last_snapshot);

    void server_disconnected_screen();
};

#endif

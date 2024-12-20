#include "screen_manager.h"

#include <algorithm>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <SDL_keycode.h>

#include "SDL2pp/Texture.hh"
#include "client/animation_data_provider.h"
#include "client/renderables/map.h"
#include "client/textures_provider.h"
#include "common/config.h"
#include "common/lobby.h"
#include "common/shared_constants.h"
#include "common/snapshot.h"

#define END_GAME_DELAY 1000

static Config& config = Config::get_instance();

const static int RATE = 1000 / config.get_client_fps();
const static bool SHOW_HUD = config.get_show_hud();


ScreenManager::ScreenManager(SDL2pp::Window& window, SoundManager& sound_manager,
                             SDL2pp::Renderer& renderer, Camera& camera, RenderableMap& map,
                             std::pair<uint8_t, uint8_t>& duck_ids, bool& play_again,
                             std::function<void(Camera&, RenderableMap&)> aditional_render):
        window(window),
        sound_manager(sound_manager),
        renderer(renderer),
        duck_ids(duck_ids),
        primary_font(DATA_PATH "/fonts/primary.ttf", 30),
        map(map),
        camera(camera),
        play_again(play_again),
        owner_started_game(false),
        aditional_render(aditional_render) {
    TexturesProvider::load_textures(renderer);
    AnimationDataProvider::load_animations_data();
}

bool ScreenManager::between_rounds_screen(Queue<Snapshot>& snapshot_q, Snapshot& last_snapshot) {
    if (last_snapshot.game_finished) {
        if (last_snapshot.ducks.size() <= 1) {
            return end_game_no_winner_screen();
        }
        return end_game_screen(last_snapshot);
    }

    if (last_snapshot.show_stats) {
        return stats_screen(snapshot_q, last_snapshot);
    }

    while (!snapshot_q.try_pop(last_snapshot)) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return false;
            }

            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_1)
                    sound_manager.toggle_mute();
                else if (event.key.keysym.sym == SDLK_2)
                    window.SetFullscreen(!(window.GetFlags() & SDL_WINDOW_FULLSCREEN));
            }
        }

        renderer.Clear();
        camera.update(last_snapshot, false);
        aditional_render(camera, map);

        renderer.Present();
        SDL_Delay(RATE);
    }
    return true;
}

void ScreenManager::render_duck_stat(const Duck& duck, SDL2pp::Rect rect,
                                     std::shared_ptr<SDL2pp::Texture> ducks_texture,
                                     AnimationData& animation_data) {

    renderer.SetDrawBlendMode(SDL_BLENDMODE_BLEND);
    renderer.SetDrawColor(0, 0, 0, 160);
    renderer.FillRect(rect);
    renderer.SetDrawBlendMode(SDL_BLENDMODE_NONE);

    SDL2pp::Rect dst_rect = animation_data.frames[0].rect;
    dst_rect.w *= 2;
    dst_rect.h *= 2;
    dst_rect.x = rect.x + 10;
    dst_rect.y = rect.y + (rect.h - dst_rect.h) / 2;

    renderer.Copy(*ducks_texture, animation_data.frames[0].rect, dst_rect);

    SDL2pp::Texture name_texture(
            renderer,
            primary_font.RenderText_Solid(duck.player_name, SDL_Color{255, 255, 255, 255}));
    renderer.Copy(name_texture, SDL2pp::NullOpt,
                  SDL2pp::Rect(SDL2pp::Point(dst_rect.x + dst_rect.w + 20,
                                             rect.y + (rect.h - name_texture.GetSize().y) / 2),
                               name_texture.GetSize()));

    SDL2pp::Texture rounds_won_texture(
            renderer, primary_font.RenderText_Solid(std::to_string(duck.rounds_won),
                                                    SDL_Color{255, 255, 255, 255}));
    renderer.Copy(
            rounds_won_texture, SDL2pp::NullOpt,
            SDL2pp::Rect(SDL2pp::Point(rect.x + rect.w - rounds_won_texture.GetSize().x - 10,
                                       rect.y + (rect.h - rounds_won_texture.GetSize().y) / 2),
                         rounds_won_texture.GetSize()));
}

bool ScreenManager::stats_screen(Queue<Snapshot>& snapshot_q, Snapshot& last_snapshot) {
    std::shared_ptr<SDL2pp::Texture> duck_texture(TexturesProvider::get_texture("duck"));
    std::vector<AnimationData> animations(MAX_DUCKS);

    std::vector<Duck> ducks;

    for (auto& duck: last_snapshot.ducks) {
        animations[duck.duck_id] = AnimationDataProvider::get_animation_data(
                "duck_" + std::to_string(duck.duck_id) + "_standing");
        ducks.push_back(duck);
    }

    std::sort(ducks.begin(), ducks.end(),
              [](const Duck& a, const Duck& b) { return a.rounds_won > b.rounds_won; });

    SDL2pp::Texture info(renderer, primary_font.RenderText_Solid("Partial leaderboard",
                                                                 SDL_Color{255, 255, 255, 255}));

    while (!snapshot_q.try_pop(last_snapshot)) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return false;
            }

            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_1)
                    sound_manager.toggle_mute();
                else if (event.key.keysym.sym == SDLK_2)
                    window.SetFullscreen(!(window.GetFlags() & SDL_WINDOW_FULLSCREEN));
            }
        }

        renderer.Clear();

        camera.update(last_snapshot, false);
        aditional_render(camera, map);

        int rect_width = renderer.GetOutputWidth() * 0.8;
        int rect_height = 80;
        int start_x = (renderer.GetOutputWidth() - rect_width) / 2;
        int start_y = renderer.GetOutputHeight() * 0.1;

        renderer.Copy(
                info, SDL2pp::NullOpt,
                SDL2pp::Rect(SDL2pp::Point(renderer.GetOutputWidth() / 2 - info.GetSize().x / 2,
                                           start_y),
                             info.GetSize()));

        for (size_t i = 0; i < ducks.size(); ++i) {
            SDL2pp::Rect duck_rect(start_x,
                                   start_y + info.GetHeight() * 1.3 + i * (rect_height + 10),
                                   rect_width, rect_height);
            render_duck_stat(ducks[i], duck_rect, duck_texture, animations[ducks[i].duck_id]);
        }

        renderer.Present();
        SDL_Delay(RATE);
    }

    return true;
}

bool ScreenManager::end_game_screen(Snapshot& last_snapshot) {
    std::shared_ptr<SDL2pp::Texture> duck_texture(TexturesProvider::get_texture("duck"));
    std::vector<AnimationData> animations(MAX_DUCKS);
    std::vector<Duck> ducks;
    for (auto& duck: last_snapshot.ducks) {
        animations[duck.duck_id] = AnimationDataProvider::get_animation_data(
                "duck_" + std::to_string(duck.duck_id) + "_standing");
        ducks.push_back(duck);
    }
    std::sort(ducks.begin(), ducks.end(),
              [](const Duck& a, const Duck& b) { return a.rounds_won > b.rounds_won; });

    std::string winner_name = ducks[0].player_name;
    SDL2pp::Texture winner_text(
            renderer,
            primary_font.RenderText_Solid("Winner: " + winner_name, SDL_Color{255, 255, 255, 255}));

    SDL2pp::Texture exit_text(
            renderer,
            primary_font.RenderText_Solid("Press ESC to exit", SDL_Color{255, 255, 255, 255}));

    SDL2pp::Texture play_again_text(
            renderer,
            primary_font.RenderText_Solid("Press r to play again", SDL_Color{255, 255, 255, 255}));

    while (true) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                return false;

            if (event.type != SDL_KEYDOWN) {
                continue;
            }

            const auto& key = event.key.keysym.sym;

            if (key == SDLK_ESCAPE) {
                return false;
            } else if (key == SDLK_1) {
                sound_manager.toggle_mute();
            } else if (key == SDLK_2) {
                window.SetFullscreen(!(window.GetFlags() & SDL_WINDOW_FULLSCREEN));
            } else if (key == SDLK_r) {
                play_again = true;
                return false;
            }
        }

        renderer.Clear();
        camera.update(last_snapshot, false);
        aditional_render(camera, map);

        int rect_width = renderer.GetOutputWidth() * 0.8;
        int rect_height = 80;
        int start_x = (renderer.GetOutputWidth() - rect_width) / 2;
        int winner_text_y = renderer.GetOutputHeight() * 0.05;

        renderer.Copy(winner_text, SDL2pp::NullOpt,
                      SDL2pp::Rect(SDL2pp::Point(renderer.GetOutputWidth() / 2 -
                                                         winner_text.GetSize().x / 2,
                                                 winner_text_y),
                                   winner_text.GetSize()));

        renderer.Copy(
                exit_text, SDL2pp::NullOpt,
                SDL2pp::Rect(
                        SDL2pp::Point(renderer.GetOutputWidth() / 2 - exit_text.GetSize().x / 2,
                                      renderer.GetOutputHeight() - exit_text.GetSize().y - 100),
                        exit_text.GetSize()));

        renderer.Copy(play_again_text, SDL2pp::NullOpt,
                      SDL2pp::Rect(SDL2pp::Point(renderer.GetOutputWidth() / 2 -
                                                         play_again_text.GetSize().x / 2,
                                                 renderer.GetOutputHeight() -
                                                         play_again_text.GetSize().y - 30),
                                   play_again_text.GetSize()));

        for (size_t i = 0; i < ducks.size(); ++i) {
            SDL2pp::Rect duck_rect(
                    start_x, winner_text_y + winner_text.GetHeight() * 1.3 + i * (rect_height + 10),
                    rect_width, rect_height);
            render_duck_stat(ducks[i], duck_rect, duck_texture, animations[ducks[i].duck_id]);
        }

        renderer.Present();

        SDL_Delay(RATE);
    }
    return false;
}

bool ScreenManager::end_game_no_winner_screen() {
    std::string message = "Not enough players to continue";
    if (!owner_started_game) {
        message = "Owner left the game";
    }

    SDL2pp::Texture info(renderer,
                         primary_font.RenderText_Solid(message, SDL_Color{255, 255, 255, 255}));


    SDL2pp::Texture exit_text(
            renderer,
            primary_font.RenderText_Solid("Press ESC to exit", SDL_Color{255, 255, 255, 255}));


    SDL2pp::Texture play_again_text(
            renderer,
            primary_font.RenderText_Solid("Press r to play again", SDL_Color{255, 255, 255, 255}));

    int screen_width = renderer.GetOutputWidth();
    int screen_height = renderer.GetOutputHeight();
    SDL2pp::Rect background_rect(0, 0, screen_width, screen_height);
    while (true) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                return false;

            if (event.type != SDL_KEYDOWN) {
                continue;
            }

            const auto& key = event.key.keysym.sym;

            if (key == SDLK_ESCAPE) {
                return false;
            } else if (key == SDLK_1) {
                sound_manager.toggle_mute();
            } else if (key == SDLK_2) {
                window.SetFullscreen(!(window.GetFlags() & SDL_WINDOW_FULLSCREEN));
            } else if (key == SDLK_r) {
                play_again = true;
                return false;
            }
        }

        screen_width = renderer.GetOutputWidth();
        screen_height = renderer.GetOutputHeight();
        background_rect.w = screen_width;
        background_rect.h = screen_height;

        renderer.Clear();
        camera.update({}, false);
        map.render(renderer, camera);
        renderer.SetDrawBlendMode(SDL_BLENDMODE_BLEND);
        renderer.SetDrawColor(0, 0, 0, 50);
        renderer.FillRect(background_rect);
        renderer.SetDrawBlendMode();

        renderer.Copy(info, SDL2pp::NullOpt,
                      SDL2pp::Rect(SDL2pp::Point(screen_width / 2 - info.GetSize().x / 2,
                                                 screen_height / 2 - info.GetSize().y / 2),
                                   info.GetSize()));

        renderer.Copy(exit_text, SDL2pp::NullOpt,
                      SDL2pp::Rect(SDL2pp::Point(screen_width / 2 - exit_text.GetSize().x / 2,
                                                 screen_height - exit_text.GetSize().y - 100),
                                   exit_text.GetSize()));


        renderer.Copy(play_again_text, SDL2pp::NullOpt,
                      SDL2pp::Rect(SDL2pp::Point(renderer.GetOutputWidth() / 2 -
                                                         play_again_text.GetSize().x / 2,
                                                 renderer.GetOutputHeight() -
                                                         play_again_text.GetSize().y - 30),
                                   play_again_text.GetSize()));

        renderer.Present();

        SDL_Delay(RATE);
    }
    return false;
}

void ScreenManager::server_disconnected_screen() {
    SDL2pp::Texture info(renderer, primary_font.RenderText_Solid("Server disconnected",
                                                                 SDL_Color{255, 255, 255, 255}));

    SDL2pp::Texture exit_text(
            renderer,
            primary_font.RenderText_Solid("Press ESC to exit", SDL_Color{255, 255, 255, 255}));

    int screen_width = renderer.GetOutputWidth();
    int screen_height = renderer.GetOutputHeight();
    SDL2pp::Rect background_rect(0, 0, screen_width, screen_height);
    while (true) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                return;

            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    return;
                if (event.key.keysym.sym == SDLK_1)
                    sound_manager.toggle_mute();
                else if (event.key.keysym.sym == SDLK_2)
                    window.SetFullscreen(!(window.GetFlags() & SDL_WINDOW_FULLSCREEN));
            }
        }

        screen_width = renderer.GetOutputWidth();
        screen_height = renderer.GetOutputHeight();
        background_rect.w = screen_width;
        background_rect.h = screen_height;

        renderer.Clear();

        camera.update({}, false);
        aditional_render(camera, map);
        renderer.SetDrawBlendMode(SDL_BLENDMODE_BLEND);
        renderer.SetDrawColor(0, 0, 0, 50);
        renderer.FillRect(background_rect);
        renderer.SetDrawBlendMode();

        renderer.Copy(info, SDL2pp::NullOpt,
                      SDL2pp::Rect(SDL2pp::Point(screen_width / 2 - info.GetSize().x / 2,
                                                 screen_height / 2 - info.GetSize().y / 2),
                                   info.GetSize()));

        renderer.Copy(exit_text, SDL2pp::NullOpt,
                      SDL2pp::Rect(SDL2pp::Point(screen_width / 2 - exit_text.GetSize().x / 2,
                                                 screen_height - exit_text.GetSize().y - 100),
                                   exit_text.GetSize()));


        renderer.Present();

        SDL_Delay(RATE);
    }
}

void ScreenManager::show_lobby_text(Snapshot& last_snapshot) {
    owner_started_game = owner_started_game || !last_snapshot.boxes.empty();

    std::string message = "Break the boxes to start";
    if (!owner_started_game)
        message = "Waiting for owner to start";
    else if (last_snapshot.boxes.size() == 0)
        message = "Starting game...";

    SDL2pp::Texture info(renderer,
                         primary_font.RenderText_Solid(message, SDL_Color{255, 255, 255, 255}));

    SDL2pp::Rect info_rect;

    info_rect.w = info.GetSize().x * 0.6;
    info_rect.h = info.GetSize().y * 0.6;
    info_rect.x = LOBBY_MAP_X + (LOBBY_MAP_WIDTH - info_rect.w) / 2;
    info_rect.y = LOBBY_MAP_Y + (LOBBY_MAP_HEIGHT - info_rect.h) / 2;
    camera.transform_rect(info_rect);
    renderer.Copy(info, SDL2pp::NullOpt, info_rect);

    for (const auto& duck: last_snapshot.ducks) {
        SDL2pp::Texture duck_name(
                renderer,
                primary_font.RenderText_Solid(duck.player_name, SDL_Color{255, 255, 255, 255}));
        SDL2pp::Rect duck_name_rect;
        duck_name_rect.w = duck_name.GetSize().x * 0.27;
        duck_name_rect.h = duck_name.GetSize().y * 0.27;

        duck_name_rect.x = LOBBY_MAP_X;
        duck_name_rect.x += duck.duck_id % 2 == 0 ? 75 - duck_name_rect.w / 2 :
                                                    LOBBY_MAP_WIDTH - 80 - duck_name_rect.w / 2;
        duck_name_rect.y = LOBBY_MAP_Y + (LOBBY_MAP_HEIGHT / 2) - 19;
        duck_name_rect.y += duck.duck_id >= 2 ? ((LOBBY_MAP_HEIGHT / 2) - 8) : 0;

        if (duck.duck_id == 0 && !owner_started_game) {
            std::shared_ptr<SDL2pp::Texture> computer_texture(
                    TexturesProvider::get_texture("computer"));
            SDL2pp::Rect computer_rect(
                    LOBBY_MAP_X + 60, duck_name_rect.y - 29, 20,
                    (20.0f / computer_texture->GetWidth()) * computer_texture->GetHeight());
            camera.transform_rect(computer_rect);
            renderer.Copy(*computer_texture, SDL2pp::NullOpt, computer_rect);
        }

        camera.transform_rect(duck_name_rect);
        renderer.Copy(duck_name, SDL2pp::NullOpt, duck_name_rect);
    }
}

bool ScreenManager::round_start_screen(Queue<Snapshot>& snapshot_q, Snapshot& last_snapshot) {
    const uint time_per_number = COUNTDOWN_TIME / 3;

    for (uint8_t i = 3; i > 0; i--) {
        uint start_time = SDL_GetTicks();
        uint elapsed_time = 0;

        SDL2pp::Texture countdown_texture(
                renderer,
                primary_font.RenderText_Solid(std::to_string(i), SDL_Color{255, 255, 255, 255}));

        while (elapsed_time < time_per_number) {
            // Si hay una nueva snapshot, salimos directamente
            if (snapshot_q.try_pop(last_snapshot)) {
                return true;
            }

            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    return false;
                }
                if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_1) {
                        sound_manager.toggle_mute();
                    } else if (event.key.keysym.sym == SDLK_2) {
                        window.SetFullscreen(!(window.GetFlags() & SDL_WINDOW_FULLSCREEN));
                    }
                }
            }

            camera.update(last_snapshot, true);
            aditional_render(camera, map);

            int window_width = renderer.GetOutputWidth();
            int window_height = renderer.GetOutputHeight();

            int text_width = countdown_texture.GetWidth() * 2;
            int text_height = countdown_texture.GetHeight() * 2;
            SDL2pp::Rect dst_rect((window_width - text_width) / 2,
                                  (window_height - text_height) / 2, text_width * 2,
                                  text_height * 2);

            renderer.Copy(countdown_texture, SDL2pp::NullOpt, dst_rect);

            if (SHOW_HUD)
                show_hud(last_snapshot);

            renderer.Present();

            SDL_Delay(RATE);

            // no necesitamos que el tiempo de espera sea exacto ni respetar
            // el framerate porque es solo texto
            elapsed_time = SDL_GetTicks() - start_time;
        }
    }

    return true;
}

void ScreenManager::show_hud(Snapshot& last_snapshot) {
    // buscamos el pato del jugador con un iterador por referencia
    auto it = std::find_if(last_snapshot.ducks.begin(), last_snapshot.ducks.end(),
                           [this](const Duck& duck) { return duck.duck_id == duck_ids.first; });

    if (it != last_snapshot.ducks.end()) {
        SDL2pp::Texture hp(renderer, primary_font.RenderText_Solid(std::to_string(it->duck_hp),
                                                                   SDL_Color{255, 255, 255, 255}));
        SDL2pp::Rect rect(4, -3, 64, 64);
        renderer.Copy(*TexturesProvider::get_texture("duck"),
                      AnimationDataProvider::get_animation_data(
                              "duck_" + std::to_string(duck_ids.first) + "_standing")
                              .frames[0]
                              .rect,
                      rect);
        renderer.Copy(hp, SDL2pp::NullOpt, SDL2pp::Rect(64, 20, hp.GetSize().x, hp.GetSize().y));
    }

    if (duck_ids.second == INVALID_DUCK_ID)
        return;

    it = std::find_if(last_snapshot.ducks.begin(), last_snapshot.ducks.end(),
                      [this](const Duck& duck) { return duck.duck_id == duck_ids.second; });

    if (it != last_snapshot.ducks.end()) {
        SDL2pp::Texture hp(renderer, primary_font.RenderText_Solid(std::to_string(it->duck_hp),
                                                                   SDL_Color{255, 255, 255, 255}));
        SDL2pp::Rect rect(renderer.GetOutputWidth() - 168, -3, 64, 64);
        renderer.Copy(*TexturesProvider::get_texture("duck"),
                      AnimationDataProvider::get_animation_data(
                              "duck_" + std::to_string(duck_ids.second) + "_standing")
                              .frames[0]
                              .rect,
                      rect);
        renderer.Copy(
                hp, SDL2pp::NullOpt,
                SDL2pp::Rect(renderer.GetOutputWidth() - 104, 20, hp.GetSize().x, hp.GetSize().y));
    }
}

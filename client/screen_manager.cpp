#include "screen_manager.h"

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <SDL_keycode.h>

#include "client/animation_data_provider.h"
#include "client/renderables/map.h"
#include "client/textures_provider.h"
#include "common/config.h"
#include "common/lobby.h"
#include "common/snapshot.h"
#include "common/shared_constants.h"

#define END_GAME_DELAY 1000

static Config& config = Config::get_instance();

const static int RATE = 1000 / config.get_client_fps();


ScreenManager::ScreenManager(SDL2pp::Window& window, SoundManager& sound_manager,
                             SDL2pp::Renderer& renderer, Camera& camera, RenderableMap& map,
                             std::pair<uint8_t, uint8_t>& duck_ids, bool& play_again):
        window(window),
        sound_manager(sound_manager),
        renderer(renderer),
        duck_ids(duck_ids),
        primary_font(DATA_PATH "/fonts/primary.ttf", 30),
        map(map),
        camera(camera),
        play_again(play_again) {}

bool ScreenManager::waiting_screen(Queue<Snapshot>& snapshot_q, Snapshot& last_snapshot) {
    std::shared_ptr<SDL2pp::Texture> duck_texture(TexturesProvider::get_texture("duck"));

    AnimationData duck_data_1(AnimationDataProvider::get_animation_data(
            "duck_" + std::to_string(duck_ids.first) + "_standing"));
    AnimationData duck_data_2;
    if (duck_ids.second != INVALID_DUCK_ID) {
        duck_data_2 = AnimationDataProvider::get_animation_data(
                "duck_" + std::to_string(duck_ids.second) + "_standing");
    }

    SDL2pp::Texture info_1(
            renderer, primary_font.RenderText_Solid("P1: Duck ID " + std::to_string(duck_ids.first),
                                                    SDL_Color{255, 255, 255, 255}));
    SDL2pp::Texture info_2(renderer, primary_font.RenderText_Solid(
                                             "P2: Duck ID " + std::to_string(duck_ids.second),
                                             SDL_Color{255, 255, 255, 255}));

    std::array<SDL2pp::Texture, 4> waiting_texts = {
            SDL2pp::Texture(renderer, primary_font.RenderText_Solid("Waiting for players   ",
                                                                    SDL_Color{255, 255, 255, 255})),
            SDL2pp::Texture(renderer, primary_font.RenderText_Solid("Waiting for players.  ",
                                                                    SDL_Color{255, 255, 255, 255})),
            SDL2pp::Texture(renderer, primary_font.RenderText_Solid("Waiting for players.. ",
                                                                    SDL_Color{255, 255, 255, 255})),
            SDL2pp::Texture(renderer,
                            primary_font.RenderText_Solid("Waiting for players...",
                                                          SDL_Color{255, 255, 255, 255}))};

    uint8_t it = 0;
    uint8_t it_since_change = 0;

    int window_width = renderer.GetOutputWidth();
    int window_height = renderer.GetOutputHeight();
    float window_aspect_ratio = (float)window_width / window_height;

    SDL2pp::Rect dst_1 = duck_data_1.frames[0].rect;
    SDL2pp::Rect dst_2;
    dst_1.w *= 5;
    dst_1.h *= 5;
    if (duck_ids.second == INVALID_DUCK_ID) {
        dst_1.x = window_width / 2 - dst_1.w / 2;
        dst_1.y = window_height / 2;
    } else {
        dst_1.x = window_width / 4 - dst_1.w / 2;
        dst_1.y = window_height / 2;
        dst_2 = duck_data_2.frames[0].rect;
        dst_2.w *= 5;
        dst_2.h *= 5;
        dst_2.x = window_width / 4 * 3 - dst_2.w / 2;
        dst_2.y = window_height / 2;
    }

    SDL2pp::Rect background_rect;
    SDL2pp::Texture background_texture(renderer, DATA_PATH "/backgrounds/lobby.png");

    int texture_width = background_texture.GetWidth();
    int texture_height = background_texture.GetHeight();
    float texture_aspect_ratio = (float)texture_width / texture_height;

    if (window_aspect_ratio > texture_aspect_ratio) {
        background_rect.w = window_width;
        background_rect.h = static_cast<int>(background_rect.w / texture_aspect_ratio);
    } else {
        background_rect.h = window_height;
        background_rect.w = static_cast<int>(background_rect.h * texture_aspect_ratio);
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

        window_width = renderer.GetOutputWidth();
        window_height = renderer.GetOutputHeight();
        window_aspect_ratio = (float)window_width / window_height;

        if (duck_ids.second == INVALID_DUCK_ID) {
            dst_1.x = window_width / 2 - dst_1.w / 2;
            dst_1.y = window_height / 2;
        } else {
            dst_1.x = window_width / 4 - dst_1.w / 2;
            dst_1.y = window_height / 2;
            dst_2.x = window_width / 4 * 3 - dst_2.w / 2;
            dst_2.y = window_height / 2;
        }


        if (window_aspect_ratio > texture_aspect_ratio) {
            background_rect.w = window_width;
            background_rect.h = static_cast<int>(background_rect.w / texture_aspect_ratio);
        } else {
            background_rect.h = window_height;
            background_rect.w = static_cast<int>(background_rect.h * texture_aspect_ratio);
        }

        renderer.Copy(background_texture, SDL2pp::NullOpt, background_rect);

        renderer.SetDrawBlendMode(SDL_BLENDMODE_BLEND);
        renderer.SetDrawColor(0, 0, 0, 80);
        renderer.FillRect(background_rect);
        renderer.SetDrawBlendMode();

        renderer.Copy(*duck_texture, duck_data_1.frames[0].rect, dst_1);
        renderer.Copy(info_1, SDL2pp::NullOpt,
                      SDL2pp::Rect(SDL2pp::Point(dst_1.x + dst_1.w / 2 - info_1.GetSize().x / 2,
                                                 dst_1.y - info_1.GetSize().y - 10),
                                   info_1.GetSize()));

        if (duck_ids.second != INVALID_DUCK_ID) {
            renderer.Copy(*duck_texture, duck_data_2.frames[0].rect, dst_2);
            renderer.Copy(info_2, SDL2pp::NullOpt,
                          SDL2pp::Rect(SDL2pp::Point(dst_2.x + dst_2.w / 2 - info_2.GetSize().x / 2,
                                                     dst_2.y - info_2.GetSize().y - 10),
                                       info_2.GetSize()));
        }

        renderer.Copy(waiting_texts[it], SDL2pp::NullOpt,
                      SDL2pp::Rect(SDL2pp::Point(renderer.GetOutputWidth() / 2 -
                                                         waiting_texts[it].GetSize().x / 2,
                                                 renderer.GetOutputHeight() -
                                                         waiting_texts[it].GetSize().y - 30),
                                   waiting_texts[it].GetSize()));

        renderer.Present();

        it_since_change++;
        if (it_since_change == 40) {
            it = (it + 1) % waiting_texts.size();
            it_since_change = 0;
        }

        SDL_Delay(RATE);
    }

    return true;
}

bool ScreenManager::initial_screen(Queue<Snapshot>& snapshot_q, Snapshot& last_snapshot) {
    std::shared_ptr<SDL2pp::Texture> duck_texture(TexturesProvider::get_texture("duck"));
    std::vector<Duck>& ducks = last_snapshot.ducks;
    std::vector<AnimationData> duck_animations;

    std::sort(ducks.begin(), ducks.end(),
              [](const Duck& a, const Duck& b) { return a.duck_id < b.duck_hp; });

    std::transform(ducks.begin(), ducks.end(), std::back_inserter(duck_animations),
                   [](const auto& duck) {
                       return AnimationDataProvider::get_animation_data(
                               "duck_" + std::to_string(duck.duck_id) + "_standing");
                   });

    int screen_width = renderer.GetOutputWidth();
    int screen_height = renderer.GetOutputHeight();
    SDL2pp::Rect background_rect(0, 0, screen_width, screen_height);

    while (!snapshot_q.try_pop(last_snapshot)) {
        screen_width = renderer.GetOutputWidth();
        screen_height = renderer.GetOutputHeight();
        background_rect.w = screen_width;
        background_rect.h = screen_height;

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
        map.render(renderer, camera);
        renderer.SetDrawBlendMode(SDL_BLENDMODE_BLEND);
        renderer.SetDrawColor(0, 0, 0, 50);
        renderer.FillRect(background_rect);
        renderer.SetDrawBlendMode();

        camera.update(last_snapshot, false);
        map.render(renderer, camera);

        int section_width = screen_width / 2;
        int section_height = screen_height / 2;

        for (size_t i = 0; i < ducks.size(); ++i) {
            const Duck& duck = ducks[i];
            const AnimationData& animation_data = duck_animations[i];

            int row = i / 2;
            int col = i % 2;

            int center_x = col * section_width + section_width / 2;
            int center_y = row * section_height + section_height / 2;

            SDL2pp::Rect duck_dst_rect = animation_data.frames[0].rect;
            duck_dst_rect.w *= 4;
            duck_dst_rect.h *= 4;
            duck_dst_rect.x = center_x - duck_dst_rect.w / 2;
            duck_dst_rect.y = center_y - duck_dst_rect.h / 2;
            renderer.Copy(*duck_texture, animation_data.frames[0].rect, duck_dst_rect);

            SDL2pp::Texture player_name_texture(
                    renderer,
                    primary_font.RenderText_Solid(duck.player_name, SDL_Color{255, 255, 255, 255}));
            renderer.Copy(player_name_texture, SDL2pp::NullOpt,
                          SDL2pp::Rect(SDL2pp::Point(center_x - player_name_texture.GetSize().x / 2,
                                                     duck_dst_rect.y - 10),
                                       player_name_texture.GetSize()));
        }

        renderer.Present();
        SDL_Delay(RATE);
    }
    return true;
}

bool ScreenManager::between_rounds_screen(Queue<Snapshot>& snapshot_q, Snapshot& last_snapshot) {
    if (last_snapshot.game_finished) {
        std::cout << "Game finished" << std::endl;
        if (last_snapshot.ducks.size() == 0) {
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
        map.render(renderer, camera);
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
        map.render(renderer, camera);

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
        map.render(renderer, camera);

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
    SDL2pp::Texture info(renderer, primary_font.RenderText_Solid("Game terminated with no winner",
                                                                 SDL_Color{255, 255, 255, 255}));

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


        renderer.Present();

        SDL_Delay(RATE);
    }
}

void ScreenManager::show_lobby_text(Snapshot& last_snapshot) {
    SDL2pp::Texture info(renderer, primary_font.RenderText_Solid("Break the boxes to start",
                                                                 SDL_Color{255, 255, 255, 255}));

    SDL2pp::Rect info_rect;

    info_rect.w = info.GetSize().x*0.7;
    info_rect.h = info.GetSize().y*0.7;
    info_rect.x = LOBBY_MAP_X + (LOBBY_MAP_WIDTH - info_rect.w) / 2;
    info_rect.y = LOBBY_MAP_Y + (LOBBY_MAP_HEIGHT - info_rect.h) / 2;
    camera.transform_rect(info_rect);
    renderer.Copy(info, SDL2pp::NullOpt, info_rect);

    for (const auto& duck: last_snapshot.ducks) {
        SDL2pp::Texture duck_name(renderer, primary_font.RenderText_Solid(duck.player_name,
                                                                         SDL_Color{255, 255, 255, 255}));
        SDL2pp::Rect duck_name_rect;
        duck_name_rect.w = duck_name.GetSize().x*0.25;
        duck_name_rect.h = duck_name.GetSize().y*0.25;

        duck_name_rect.x = LOBBY_MAP_X;
        duck_name_rect.x += duck.duck_id % 2 == 0 ? 75 - duck_name_rect.w/2 : LOBBY_MAP_WIDTH - 80 - duck_name_rect.w/2;
        duck_name_rect.y = LOBBY_MAP_Y + (LOBBY_MAP_HEIGHT / 2) - 19;
        duck_name_rect.y += duck.duck_id >= 2 ? ((LOBBY_MAP_HEIGHT / 2) - 8) : 0;

        camera.transform_rect(duck_name_rect);
        renderer.Copy(duck_name, SDL2pp::NullOpt, duck_name_rect);
    }

}

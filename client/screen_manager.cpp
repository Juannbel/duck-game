#include "screen_manager.h"
#include <cstdint>
#include <memory>
#include <string>
#include "client/animation_data_provider.h"
#include "client/textures_provider.h"
#include "common/snapshot.h"
#include "config.h"

ScreenManager::ScreenManager(SDL2pp::Renderer& renderer, uint8_t duck_id) :
    duck_id(duck_id),
    renderer(renderer),
    primary_font(DATA_PATH "/fonts/primary.ttf", 30) {};

bool ScreenManager::waiting_screen(Queue<Snapshot> &snapshot_q, Snapshot &last_snapshot) {
    std::shared_ptr<SDL2pp::Texture> duck_texture(TexturesProvider::get_texture("duck"));
    AnimationData duck_data(AnimationDataProvider::get_animation_data("duck_" + std::to_string(duck_id) + "_standing"));

    SDL2pp::Texture info(renderer, primary_font.RenderText_Solid("Your duck id is " + std::to_string(duck_id), SDL_Color{255, 255, 255, 255}));
    SDL2pp::Texture waiting_0(renderer, primary_font.RenderText_Solid("Waiting for players   ", SDL_Color{255, 255, 255, 255}));
    SDL2pp::Texture waiting_1(renderer, primary_font.RenderText_Solid("Waiting for players.  ", SDL_Color{255, 255, 255, 255}));
    SDL2pp::Texture waiting_2(renderer, primary_font.RenderText_Solid("Waiting for players.. ", SDL_Color{255, 255, 255, 255}));
    SDL2pp::Texture waiting_3(renderer, primary_font.RenderText_Solid("Waiting for players...", SDL_Color{255, 255, 255, 255}));

    std::array<SDL2pp::Texture*, 4> texts = {&waiting_0, &waiting_1, &waiting_2, &waiting_3};

    uint8_t it = 0;
    uint8_t it_since_change = 0;

    SDL2pp::Rect dst = duck_data.frames[0].rect;
    dst.w *= 5;
    dst.h *= 5;
    dst.x = renderer.GetOutputWidth() / 2 - dst.w / 2;
    dst.y = renderer.GetOutputHeight() / 4 * 3 - dst.h / 2;

    while (!snapshot_q.try_pop(last_snapshot)) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return false;
            }
        }

        renderer.Clear();

        renderer.Copy(*duck_texture, duck_data.frames[0].rect, dst);
        renderer.Copy(info, SDL2pp::NullOpt, SDL2pp::Rect(
            SDL2pp::Point(
                renderer.GetOutputWidth() / 2 - info.GetSize().x / 2,
                renderer.GetOutputHeight() / 4 - info.GetSize().y / 2),
            info.GetSize()));

        renderer.Copy(*texts[it], SDL2pp::NullOpt, SDL2pp::Rect(
            SDL2pp::Point(
                renderer.GetOutputWidth() / 2 - texts[it]->GetSize().x / 2,
                renderer.GetOutputHeight() / 2 - texts[it]->GetSize().y / 2),
            texts[it]->GetSize()));
        renderer.Present();

        it_since_change++;
        if (it_since_change == 40) {
            it = (it + 1) % texts.size();
            it_since_change = 0;
        }

        SDL_Delay(RATE);
    }

    return true;
}

bool ScreenManager::between_rounds_screen(Queue<Snapshot> &snapshot_q, Snapshot &last_snapshot, uint8_t last_map_theme) {
    std::shared_ptr<SDL2pp::Texture> duck_texture(TexturesProvider::get_texture("duck"));
    std::shared_ptr<SDL2pp::Texture> background(TexturesProvider::get_texture("background_" + std::to_string(last_map_theme)));
    std::vector<AnimationData> animations(MAX_DUCKS);

    std::vector<Duck> ducks;
    for (auto& duck : last_snapshot.ducks) {
        animations[duck.duck_id] = AnimationDataProvider::get_animation_data("duck_" + std::to_string(duck.duck_id) + "_standing");
        ducks.push_back(duck);
    }

    std::sort(ducks.begin(), ducks.end(), [](const Duck& a, const Duck& b) {
        return a.duck_hp > b.duck_hp;
    });

    SDL2pp::Texture info(renderer, primary_font.RenderText_Solid("Round finished", SDL_Color{255, 255, 255, 255}));

    while (!snapshot_q.try_pop(last_snapshot)) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return false;
            }
        }

        renderer.Clear();

        SDL2pp::Rect background_rect;
        float bg_aspect_ratio = static_cast<float>(background->GetWidth()) / background->GetHeight();
        float window_aspect_ratio = static_cast<float>(renderer.GetOutputWidth()) / renderer.GetOutputHeight();

        if (window_aspect_ratio > bg_aspect_ratio) {
            background_rect.x = 0;
            background_rect.y = (renderer.GetOutputHeight() - background_rect.h) / 2;
            background_rect.w = renderer.GetOutputWidth();
            background_rect.h = static_cast<int>(renderer.GetOutputWidth() / bg_aspect_ratio);
        } else {
            background_rect.x = (renderer.GetOutputWidth() - background_rect.w) / 2;
            background_rect.y = 0;
            background_rect.w = static_cast<int>(renderer.GetOutputHeight() * bg_aspect_ratio);
            background_rect.h = renderer.GetOutputHeight();
        }

        renderer.Copy(*background, SDL2pp::NullOpt, background_rect);

        renderer.SetDrawBlendMode(SDL_BLENDMODE_BLEND);
        renderer.SetDrawColor(0, 0, 0, 100);
        renderer.FillRect(SDL2pp::Rect(0, 0, renderer.GetOutputWidth(), renderer.GetOutputHeight()));
        renderer.SetDrawBlendMode(SDL_BLENDMODE_NONE);

        int rect_width = renderer.GetOutputWidth() * 0.8;
        int rect_height = 80;
        int start_x = (renderer.GetOutputWidth() - rect_width) / 2;
        int start_y = renderer.GetOutputHeight() / 4;

        renderer.Copy(info, SDL2pp::NullOpt, SDL2pp::Rect(
            SDL2pp::Point(
                renderer.GetOutputWidth() / 2 - info.GetSize().x / 2,
                start_y - 50),
            info.GetSize()));

        // Renderizar cada pato en su rectángulo
        for (size_t i = 0; i < ducks.size(); ++i) {
            Duck& duck = ducks[i];

            SDL2pp::Rect duck_rect(start_x, start_y + i * (rect_height + 10), rect_width, rect_height);
            renderer.SetDrawBlendMode(SDL_BLENDMODE_BLEND);
            renderer.SetDrawColor(0, 0, 0, 160);
            renderer.FillRect(duck_rect);
            renderer.SetDrawBlendMode(SDL_BLENDMODE_NONE);

            SDL2pp::Rect dst_rect = animations[duck.duck_id].frames[0].rect;
            dst_rect.w *= 2;
            dst_rect.h *= 2;
            dst_rect.x = duck_rect.x + 10;
            dst_rect.y = duck_rect.y + (rect_height - dst_rect.h) / 2;

            renderer.Copy(*duck_texture, animations[duck.duck_id].frames[0].rect, dst_rect);

            SDL2pp::Texture name_texture(renderer, primary_font.RenderText_Solid("Duck " + std::to_string(duck.duck_id), SDL_Color{255, 255, 255, 255}));
            renderer.Copy(name_texture, SDL2pp::NullOpt, SDL2pp::Rect(
                SDL2pp::Point(dst_rect.x + dst_rect.w + 20, duck_rect.y + (rect_height - name_texture.GetSize().y) / 2),
                name_texture.GetSize()));

            SDL2pp::Texture hp_texture(renderer, primary_font.RenderText_Solid(std::to_string(duck.duck_hp), SDL_Color{255, 255, 255, 255}));
            renderer.Copy(hp_texture, SDL2pp::NullOpt, SDL2pp::Rect(
                SDL2pp::Point(duck_rect.x + rect_width - hp_texture.GetSize().x - 10, duck_rect.y + (rect_height - hp_texture.GetSize().y) / 2),
                hp_texture.GetSize()));
        }

        renderer.Present();
        SDL_Delay(RATE);
    }

    return true;
}

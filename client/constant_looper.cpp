#include "constant_looper.h"

#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>
#include <SDL_events.h>
#include <SDL_keycode.h>

#include "SDL2pp/Chunk.hh"
#include "SDL2pp/Font.hh"
#include "SDL2pp/Music.hh"
#include "SDL2pp/Optional.hh"
#include "SDL2pp/Renderer.hh"
#include "SDL2pp/SDLTTF.hh"
#include "client/camera.h"
#include "client/duck_controller.h"
#include "client/renderables/animation.h"
#include "client/renderables/bullet.h"
#include "client/renderables/collectable.h"
#include "client/renderables/duck.h"
#include "client/renderables/map.h"
#include "client/textures_provider.h"
#include "animation_data_provider.h"
#include "common/map_dto.h"
#include "common/snapshot.h"

#define FPS 60
#define RATE 1000 / FPS

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 690

#define USE_CAMERA true

using SDL2pp::Renderer;
using SDL2pp::SDL;
using SDL2pp::Window;

ConstantLooper::ConstantLooper(MatchInfo& match_info, Queue<Snapshot>& snapshot_q,
                               Queue<Command>& command_q):
        duck_id(match_info.duck_id),
        snapshot_q(snapshot_q),
        command_q(command_q),
        p1_controller(duck_id, command_q, last_snapshot, {SDLK_d, SDLK_a, SDLK_w, SDLK_s, SDLK_c, SDLK_v, SDLK_e}),
        map_dto(match_info.map) {}


void ConstantLooper::run() try {
    SDL sdl(SDL_INIT_VIDEO);

    Window window("Duck game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH,
                  WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);

    Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL2pp::SDLTTF sdl_ttf;

    TexturesProvider::loadTextures(renderer);
    AnimationDataProvider::load_animations_data();

    if (!waiting_screen(renderer)) return;

    process_snapshot();

    Camera camera(renderer);
    RenderableMap map(map_dto, 0);

    bool keep_running = true;
    uint32_t t1 = SDL_GetTicks();

    while (keep_running) {
        keep_running = process_events();

        while (snapshot_q.try_pop(last_snapshot)) {}
        // Actualizar el estado de todo lo que se renderiza
        process_snapshot();

        if (USE_CAMERA) {
            camera.update(last_snapshot);
        }

        render(renderer, camera, map);

        sleep_or_catch_up(t1);
    }

} catch (std::exception& e) {
    std::cerr << "Excepction on constant looper" << e.what() << std::endl;
} catch (...) {
    std::cerr << "Unknown exception on constant looper" << std::endl;
}

void ConstantLooper::sleep_or_catch_up(uint32_t& t1) {
    uint32_t t2 = SDL_GetTicks();

    int rest = RATE - (t2 - t1);
    if (rest < 0) {
        int behind = -rest;
        int lost = behind - behind % int(RATE);

        // recuperamos los frames perdidos
        uint8_t frames_to_skip = int(lost / RATE);

        for (auto& duck: ducks_renderables) {
            duck.second->skip_frames(frames_to_skip);
        }

        t1 += lost;
    } else {
        SDL_Delay(rest);
    }

    t1 += RATE;
}

void ConstantLooper::process_snapshot() {
    for (auto& duck : last_snapshot.ducks) {
        if (ducks_renderables.find(duck.duck_id) == ducks_renderables.end()) {
            ducks_renderables[duck.duck_id] = new RenderableDuck(duck.duck_id);
        }
        ducks_renderables[duck.duck_id]->update(duck);
    }

    std::unordered_set<uint32_t> collectables_in_snapshot;
    for (const Gun& gun : last_snapshot.guns) {
        collectables_in_snapshot.insert(gun.gun_id);
        if (collectables_renderables.find(gun.gun_id) == collectables_renderables.end()) {
            collectables_renderables[gun.gun_id] = new RenderableCollectable(gun.gun_id, gun.type);
        }
        collectables_renderables[gun.gun_id]->update(gun);
    }

    for (auto it = collectables_renderables.begin(); it != collectables_renderables.end();) {
        if (collectables_in_snapshot.find(it->first) == collectables_in_snapshot.end()) {
            delete it->second;
            it = collectables_renderables.erase(it);
        } else {
            ++it;
        }
    }

    std::unordered_set<uint32_t> bullets_in_snapshot;
    for (const Bullet& bullet : last_snapshot.bullets) {
        bullets_in_snapshot.insert(bullet.bullet_id);
        if (bullets_renderables.find(bullet.bullet_id) == bullets_renderables.end()) {
            if (bullet.type == Helmet || bullet.type == Armor) continue;
            sound_manager.shoot_sound(bullet.type);
            bullets_renderables[bullet.bullet_id] = new RenderableBullet(bullet.bullet_id, bullet.type);
        }
        bullets_renderables[bullet.bullet_id]->update(bullet);
    }

    for (auto it = bullets_renderables.begin(); it != bullets_renderables.end();) {
        if (bullets_in_snapshot.find(it->first) == bullets_in_snapshot.end()) {
            delete it->second;
            it = bullets_renderables.erase(it);
        } else {
            ++it;
        }
    }
}

void ConstantLooper::render(SDL2pp::Renderer& renderer, Camera& camera, RenderableMap& map) {
    renderer.Clear();

    map.render(renderer, camera);

    for (auto& bullet: bullets_renderables) {
        bullet.second->render(renderer, camera);
    }

    for (auto& duck: ducks_renderables) {
        if (duck.first == duck_id) continue;
        duck.second->render(renderer, camera);
    }
    ducks_renderables[duck_id]->render(renderer, camera);

    for (auto& collectable: collectables_renderables) {
        collectable.second->render(renderer, camera);
    }

    renderer.Present();
}

bool ConstantLooper::waiting_screen(Renderer& renderer) {
    SDL2pp::Texture *duck_texture(TexturesProvider::getTexture("duck"));
    AnimationData duck_data(AnimationDataProvider::get_animation_data("duck_" + std::to_string(duck_id) + "_standing"));

    SDL2pp::Font font(DATA_PATH "/fonts/primary.ttf", 30);
    SDL2pp::Texture info(renderer, font.RenderText_Solid("Your duck id is " + std::to_string(duck_id), SDL_Color{255, 255, 255, 255}));
    SDL2pp::Texture waiting_0(renderer, font.RenderText_Solid("Waiting for players   ", SDL_Color{255, 255, 255, 255}));
    SDL2pp::Texture waiting_1(renderer, font.RenderText_Solid("Waiting for players.  ", SDL_Color{255, 255, 255, 255}));
    SDL2pp::Texture waiting_2(renderer, font.RenderText_Solid("Waiting for players.. ", SDL_Color{255, 255, 255, 255}));
    SDL2pp::Texture waiting_3(renderer, font.RenderText_Solid("Waiting for players...", SDL_Color{255, 255, 255, 255}));

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

bool ConstantLooper::process_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return false;
        }

        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_1) {
            sound_manager.toggle_mute();
            return true;
        }

        p1_controller.process_event(event);
        // para cuando haya un segundo jugador
        // p2_controller.process_event(event);
    }

    p1_controller.send_last_move_command();
    // p2_controller.send_last_move_command();
    return true;
}

ConstantLooper::~ConstantLooper() {
    for (auto& duck: ducks_renderables) {
        delete duck.second;
    }
    ducks_renderables.clear();

    for (auto& collectable: collectables_renderables) {
        delete collectable.second;
    }
    collectables_renderables.clear();

    for (auto& bullet: bullets_renderables) {
        delete bullet.second;
    }
    bullets_renderables.clear();
}

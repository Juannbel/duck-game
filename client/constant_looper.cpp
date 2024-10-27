#include "constant_looper.h"

#include <cmath>
#include <iostream>
#include <unordered_set>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "SDL2pp/Music.hh"
#include "client/camera.h"
#include "client/duck_controller.h"
#include "client/renderables/collectable.h"
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
        last_snapshot(snapshot_q.pop()),
        p1_controller(duck_id, command_q, last_snapshot, {SDLK_d, SDLK_a, SDLK_w, SDLK_s, SDLK_c, SDLK_v, SDLK_e}),
        map_dto(match_info.map) {}

void ConstantLooper::run() try {
    SDL sdl(SDL_INIT_VIDEO);

    Window window("Duck game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH,
                  WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);

    Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL2pp::Mixer mixer(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);

    SDL2pp::Music music(DATA_PATH "/sounds/background_music.mp3");

    mixer.SetMusicVolume(0);
    mixer.PlayMusic(music, -1);

    TexturesProvider::loadTextures(renderer);
    AnimationDataProvider::load_animations_data();

    process_snapshot();

    Camera camera(renderer);
    RenderableMap map(map_dto, TexturesProvider::getTexture("blocks"), TexturesProvider::getTexture("forest_background"));

    bool keep_running = true;
    uint32_t t1 = SDL_GetTicks();

    while (keep_running) {
        // Event processing:
        keep_running = p1_controller.process_events();

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
    // actualizar el estado de todos los renderizables
    for (auto& duck : last_snapshot.ducks) {
        if (ducks_renderables.find(duck.duck_id) == ducks_renderables.end()) {
            ducks_renderables[duck.duck_id] = new RenderableDuck(duck.duck_id);
        }
        ducks_renderables[duck.duck_id]->update(duck);
    }

    // updateadmos los collectables, y los que no esten en el snapshot los eliminamos
    std::unordered_set<int> collectables_in_snapshot;

    for (const Gun& gun : last_snapshot.guns) {
        collectables_in_snapshot.insert(gun.gun_id);
        if (collectables_renderables.find(gun.gun_id) == collectables_renderables.end()) {
            // apareció un nuevo collectable
            collectables_renderables[gun.gun_id] = new RenderableCollectable(gun.gun_id, gun.type);
        }
        collectables_renderables[gun.gun_id]->update(gun);
    }

    // eliminamos los collectables que no esten en el snapshot
    for (auto it = collectables_renderables.begin(); it != collectables_renderables.end();) {
        if (collectables_in_snapshot.find(it->first) == collectables_in_snapshot.end()) {
            delete it->second;
            it = collectables_renderables.erase(it);
        } else {
            ++it;
        }
    }
}

void ConstantLooper::render(SDL2pp::Renderer& renderer, Camera& camera, RenderableMap& map) {
    renderer.Clear();

    map.render(renderer, camera);

    for (auto& duck: ducks_renderables) {
        duck.second->render(renderer, camera);
    }

    for (auto& collectable: collectables_renderables) {
        collectable.second->render(renderer, camera);
    }

    renderer.Present();
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
}

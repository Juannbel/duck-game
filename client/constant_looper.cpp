#include "constant_looper.h"

#include <cmath>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>
#include <SDL_events.h>
#include <SDL_timer.h>

#include "SDL2pp/Music.hh"
#include "client/camera.h"
#include "client/duck_controller.h"
#include "client/renderables/collectable.h"
#include "client/renderables/map.h"
#include "client/textures_provider.h"
#include "common/map_dto.h"
#include "common/snapshot.h"

#define FPS 30
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
        p1_controller(duck_id, command_q, last_snapshot, {SDLK_d, SDLK_a, SDLK_w, SDLK_s}),
        map_dto(match_info.map) {

            // provisionalmente hago esto aca
            YAML::Node config = YAML::LoadFile(DATA_PATH "/sprites/collectables/collectables.yaml");

            int width = config["width"].as<int>();
            int height = config["height"].as<int>();

            for (const auto& collectable: config["collectables"]) {
                GunType collectable_type = string_to_collectable[collectable.first.as<std::string>()];

                FrameData frame;
                int source_x = collectable.second["x"].as<int>();
                int source_y = collectable.second["y"].as<int>();
                int x_offset_right = collectable.second["x_offset_right"].as<int>();
                int x_offset_left = collectable.second["x_offset_left"].as<int>();
                int y_offset = collectable.second["y_offset"].as<int>();

                SDL2pp::Rect rect = SDL2pp::Rect(source_x, source_y, width, height);
                frame = {rect, x_offset_right, x_offset_left, y_offset};

                collectables_frames[collectable_type] = frame;
            }
        }

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

    for (int i = 0; i < last_snapshot.players_quantity; i++) {
        Duck duck = last_snapshot.ducks[i];
        ducks_renderables[i] = new RenderableDuck(
                TexturesProvider::getTexture("duck"), DATA_PATH "/sprites/duck/frames_" + std::to_string(i) + ".yaml",
                TexturesProvider::getTexture("guns"), DATA_PATH "/sprites/guns/guns.yaml");

        ducks_renderables[i]->update_from_snapshot(duck);
    }

    for (int i = 0; i < last_snapshot.guns_quantity; i++) {
        Gun gun = last_snapshot.guns[i];
        collectables_renderables[i] = new RenderableCollectable(gun.gun_id, TexturesProvider::getTexture("collectables"), collectables_frames[gun.type]);
        collectables_renderables[i]->update_from_snapshot(gun);
    }

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

        // Clear screen
        renderer.Clear();

        map.render(renderer, camera);

        // Update de los renderizables
        for (auto& duck: ducks_renderables) {
            duck.second->update();
        }


        // Render de los renderizables
        for (auto& duck: ducks_renderables) {
            duck.second->render(renderer, camera);
        }

        for (auto& collectable: collectables_renderables) {
            collectable.second->render(renderer, camera);
        }

        renderer.Present();

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
    for (int i = 0; i < last_snapshot.players_quantity; i++) {
        Duck duck = last_snapshot.ducks[i];
        ducks_renderables[i]->update_from_snapshot(duck);
    }

    // updateadmos los collectables, y los que no esten en el snapshot los eliminamos

    std::unordered_map<int, bool> collectables_to_remove;

    for (int i = 0; i < last_snapshot.guns_quantity; i++) {
        Gun& gun = last_snapshot.guns[i];
        if (collectables_renderables.find(gun.gun_id) == collectables_renderables.end()) {
            collectables_renderables[gun.gun_id] = 
                new RenderableCollectable(gun.gun_id, TexturesProvider::getTexture("collectables"),
                                         collectables_frames[gun.type]);
        }
        collectables_renderables[gun.gun_id]->update_from_snapshot(gun);
        collectables_to_remove[gun.gun_id] = true;
    }

    for (auto it = collectables_renderables.begin(); it != collectables_renderables.end();) {
        if (collectables_to_remove.find(it->first) == collectables_to_remove.end()) {
            delete it->second;
            it = collectables_renderables.erase(it);
        } else {
            it++;
        }
    }
}


ConstantLooper::~ConstantLooper() {
    for (auto& duck: ducks_renderables) {
        delete duck.second;
    }
}

std::unordered_map<std::string, GunType> ConstantLooper::string_to_collectable {
    {"none", None},
    {"grenade", Grenade},
    {"banana", Banana},
    {"pew_pew_laser", PewPewLaser},
    {"laser_rifle", LaserRifle},
    {"ak47", Ak47},
    {"dueling_pistol", DuelingPistol},
    {"cowboy_pistol", CowboyPistol},
    {"magnum", Magnum},
    {"shootgun", Shootgun},
    {"sniper", Sniper}
};

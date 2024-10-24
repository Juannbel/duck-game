#include "constant_looper.h"

#include <cmath>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "SDL2pp/Music.hh"
#include "client/camera.h"
#include "client/duck_controller.h"
#include "client/renderables/map.h"
#include "common/snapshot.h"

#define FPS 30
#define RATE 1000 / FPS

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 690

#define USE_CAMERA true

using SDL2pp::Renderer;
using SDL2pp::SDL;
using SDL2pp::Texture;
using SDL2pp::Window;

ConstantLooper::ConstantLooper(uint8_t duck_id, Queue<Snapshot>& snapshot_q,
                               Queue<Command>& command_q):
        duck_id(duck_id),
        snapshot_q(snapshot_q),
        command_q(command_q),
        last_snapshot(snapshot_q.pop()),
        p1_controller(duck_id, command_q, last_snapshot, {SDLK_d, SDLK_a, SDLK_w, SDLK_s}) {}

void ConstantLooper::run() try {
    SDL sdl(SDL_INIT_VIDEO);

    Window window("Duck game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH,
                  WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);

    Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL2pp::Mixer mixer(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);
    
    SDL2pp::Music music(DATA_PATH "/sounds/background_music.mp3");

    mixer.SetMusicVolume(15);
    mixer.PlayMusic(music, -1);

    Texture duck_sprite(renderer, DATA_PATH "/sprites/duck/duck_sprite.png");
    Texture background(renderer, DATA_PATH "/backgrounds/forest.png");
    Texture blocks(renderer, DATA_PATH "/sprites/tiles/tiles.png");
    Texture guns(renderer, DATA_PATH "/sprites/guns/guns.png");

    // generamos un mapa
    Map map_dto;
    for (int i = 0; i < MAP_HEIGHT_BLOCKS; i++) {
        for (int j = 0; j < MAP_WIDTH_BLOCKS; j++) {
            if (i == MAP_HEIGHT_BLOCKS - 1) {
                map_dto.blocks[i][j] = BlockType::Floor;
            } else {
                map_dto.blocks[i][j] = BlockType::Empty;
            }

            if (j == 0 || j == MAP_WIDTH_BLOCKS - 1) {
                map_dto.blocks[i][j] = BlockType::Wall;
            }
        }
    }

    while (snapshot_q.try_pop(last_snapshot)) {}

    for (int i = 0; i < last_snapshot.players_quantity; i++) {
        Duck duck = last_snapshot.ducks[i];
        ducks_renderables[i] = new RenderableDuck(
                &duck_sprite, DATA_PATH "/sprites/duck/frames_" + std::to_string(i) + ".yaml",
                &guns, DATA_PATH "/sprites/guns/guns.yaml");

        ducks_renderables[i]->update_from_snapshot(duck);
    }

    // for (int i = 0; i < last_snapshot.guns_quantity; i++) {
    //     Gun gun = last_snapshot.guns[i];

    //     dropped_guns[gun.gun_id] = new RenderableGun(&guns);

    //     dropped_guns[gun.gun_id]->update_from_snapshot(gun);
    // }

    Camera camera(renderer);

    RenderableMap map(map_dto, &blocks, &background);

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

        // for (auto& gun : dropped_guns) {
        //     gun.second->update();
        // }

        // Render de los renderizables
        for (auto& duck: ducks_renderables) {
            duck.second->render(renderer, camera);
        }

        // for (auto& gun : dropped_guns) {
        //     gun.second->render(renderer, camera);
        // }

        renderer.Present();

        sleep_or_catch_up(t1);
    }

} catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
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
        std::cout << "Me quede atras"
                  << "\n";
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
}


ConstantLooper::~ConstantLooper() {
    for (auto& duck: ducks_renderables) {
        delete duck.second;
    }
}

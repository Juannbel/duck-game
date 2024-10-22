#include "constant_looper.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>
#include <SDL_keycode.h>

#include "client/camera.h"
#include "client/renderables/map.h"
#include "common/snapshot.h"

#define FPS 30
#define RATE 1000 / FPS

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

#define TILE_SIZE 16
#define WORLD_WIDTH (TILE_SIZE * 35)
#define WORLD_HEIGHT (TILE_SIZE * 20)

#define FLOOR_HEIGHT 3 * TILE_SIZE

#define DUCK_VELOCITY 4

#define USE_CAMERA true

using SDL2pp::Rect;
using SDL2pp::Renderer;
using SDL2pp::SDL;
using SDL2pp::Texture;
using SDL2pp::Window;

ConstantLooper::ConstantLooper(uint8_t duck_id, Queue<Snapshot>& snapshot_q,
                               Queue<Command>& command_q):
        duck_id(duck_id), snapshot_q(snapshot_q), command_q(command_q) {}

void ConstantLooper::run() try {
    // Initialize SDL library
    SDL sdl(SDL_INIT_VIDEO);

    Window window("Duck game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH,
                  WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);

    Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

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

    Snapshot last_snapshot;
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
    unsigned int t1 = SDL_GetTicks();

    while (keep_running) {
        // Event processing:
        keep_running = process_events(last_snapshot);
        while (snapshot_q.try_pop(last_snapshot)) {}

        // Actualizar el estado de todo lo que se renderiza
        process_snapshot(last_snapshot);

        Rect target = get_minimum_bounding_box();
        if (!USE_CAMERA) {
            target = Rect(0, 0, WORLD_WIDTH, WORLD_HEIGHT);
        }

        camera.set_target(target);

        camera.update();

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

        unsigned int t2 = SDL_GetTicks();
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

} catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
}

bool ConstantLooper::process_events(const Snapshot& last_snapshot) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT ||
            (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
            return false;
        }

        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
            // cambiar de pato (por ahora)
            duck_id = (duck_id + 1) % last_snapshot.players_quantity;
            return true;
        }

        if (last_snapshot.ducks[duck_id].duck_hp == 0) {
            return true;
        }

        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {

                case SDLK_d:
                    // si el pato no estaba corriendo, enviamos el comando de correr
                    if (!last_snapshot.ducks[duck_id].is_running ||
                        !last_snapshot.ducks[duck_id].facing_right) {
                        // last_snapshot.ducks[duck_id].facing_right = true;
                        // last_snapshot.ducks[duck_id].is_running = true;
                        command_q.push(StartMovingRight);
                    }
                    break;

                case SDLK_a:
                    if (!last_snapshot.ducks[duck_id].is_running ||
                        last_snapshot.ducks[duck_id].facing_right) {
                        // last_snapshot.ducks[duck_id].facing_right = false;
                        // last_snapshot.ducks[duck_id].is_running = true;
                        command_q.push(StartMovingLeft);
                    }
                    break;

                case SDLK_w:
                    // if (!last_snapshot.ducks[duck_id].is_jumping) {
                    //     //last_snapshot.ducks[duck_id].is_jumping = true;
                    // }
                    //  siempre lo enviamos porque este o no saltando, indica tambien que quiere
                    //  seguir aleteando
                    command_q.push(Jump);
                    break;

                case SDLK_s:
                    // if (!last_snapshot.ducks[duck_id].is_laying) {
                    //     last_snapshot.ducks[duck_id].is_laying = true;
                    // }
                    command_q.push(LayDown);
                    break;
            }

        } else if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.sym) {

                case SDLK_d:
                    // si el pato no estaba corriendo, enviamos el comando de correr
                    if (last_snapshot.ducks[duck_id].is_running &&
                        last_snapshot.ducks[duck_id].facing_right) {
                        // last_snapshot.ducks[duck_id].is_running = false;
                        command_q.push(StopMoving);
                    }
                    break;

                case SDLK_a:
                    if (last_snapshot.ducks[duck_id].is_running &&
                        !last_snapshot.ducks[duck_id].facing_right) {
                        // last_snapshot.ducks[duck_id].is_running = false;
                        command_q.push(StopMoving);
                    }
                    break;

                case SDLK_e:
                    if (last_snapshot.ducks[duck_id].facing_up) {
                        // last_snapshot.ducks[duck_id].is_jumping = false;
                        command_q.push(StopLookup);
                    }
                    break;

                case SDLK_s:
                    if (last_snapshot.ducks[duck_id].is_laying) {
                        // last_snapshot.ducks[duck_id].is_laying = false;
                        command_q.push(StandUp);
                    }
                    break;
            }
        }
    }

    return true;
}

void ConstantLooper::process_snapshot(const Snapshot& last_snapshot) {
    // actualizar el estado de todos los renderizables
    for (int i = 0; i < last_snapshot.players_quantity; i++) {
        Duck duck = last_snapshot.ducks[i];
        ducks_renderables[i]->update_from_snapshot(duck);
    }
}

Rect ConstantLooper::get_minimum_bounding_box() {
    int left = std::numeric_limits<int>::max();
    int right = std::numeric_limits<int>::min();
    int top = std::numeric_limits<int>::max();
    int bottom = std::numeric_limits<int>::min();

    for (auto& duck: ducks_renderables) {
        if (duck.second->is_dead()) {
            continue;
        }

        Rect bounding_box = duck.second->get_bounding_box();
        left = std::min(left, bounding_box.x);
        right = std::max(right, bounding_box.x + bounding_box.w);
        top = std::min(top, bounding_box.y);
        bottom = std::max(bottom, bounding_box.y + bounding_box.h);
    }

    return Rect(left, top, right - left, bottom - top);
}

ConstantLooper::~ConstantLooper() {
    for (auto& duck: ducks_renderables) {
        delete duck.second;
    }
}

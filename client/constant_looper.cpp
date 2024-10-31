#include "constant_looper.h"

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <unordered_set>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>
#include <SDL_events.h>
#include <SDL_keycode.h>
#include <SDL_render.h>
#include <SDL_video.h>

#include "client/camera.h"
#include "client/duck_controller.h"
#include "client/renderables/bullet.h"
#include "client/renderables/collectable.h"
#include "client/renderables/duck.h"
#include "client/renderables/map.h"
#include "client/screen_manager.h"
#include "client/textures_provider.h"
#include "common/map_dto.h"
#include "common/snapshot.h"

#include "animation_data_provider.h"
#include "config.h"

#define USE_CAMERA true

ConstantLooper::ConstantLooper(uint8_t duck_id, Queue<Snapshot>& snapshot_q,
                               Queue<Command>& command_q):
        duck_id(duck_id),
        sdl(SDL_INIT_VIDEO | SDL_INIT_AUDIO),
        window(WIN_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIN_WIDTH, WIN_HEIGHT,
               SDL_WINDOW_RESIZABLE),
        renderer(window, -1, SDL_RENDERER_ACCELERATED),
        screen_manager(renderer, duck_id),
        snapshot_q(snapshot_q),
        command_q(command_q),
        p1_controller(duck_id, command_q, last_snapshot, P1_CONTROLS),
        map_dto() {}

void ConstantLooper::run() try {
    TexturesProvider::load_textures(renderer);
    AnimationDataProvider::load_animations_data();

    if (!screen_manager.waiting_screen(snapshot_q, last_snapshot))
        return;
    
    process_snapshot();

    Camera camera(renderer);

    RenderableMap map(map_dto, 0);

    bool keep_running = true;
    uint32_t t1 = SDL_GetTicks();


    while (keep_running) {
        keep_running = process_events();

        while (snapshot_q.try_pop(last_snapshot)) {}

        
        if (last_snapshot.match_finished) {
           keep_running = screen_manager.between_rounds_screen(snapshot_q, last_snapshot, map, camera);
           if (!keep_running) break;
           clear_renderables();
           process_snapshot();
           map.update(map_dto, 0);
           camera.update(last_snapshot);
           continue;
        }
        

        // Actualizar el estado de todo lo que se renderiza
        process_snapshot();

        if (USE_CAMERA) {
            camera.update(last_snapshot);
        }

        render(camera, map);

        sleep_or_catch_up(t1);
    }

} catch (std::exception& e) {
    std::cerr << "Excepction on constant looper " << e.what() << std::endl;
} catch (...) {
    std::cerr << "Unknown exception on constant looper " << std::endl;
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
    for (auto& map : last_snapshot.maps) {
        map_dto = map;
    }
    for (auto& duck: last_snapshot.ducks) {
        if (ducks_renderables.find(duck.duck_id) == ducks_renderables.end()) {
            ducks_renderables[duck.duck_id] = std::make_unique<RenderableDuck>(duck.duck_id);
        }
        ducks_renderables[duck.duck_id]->update(duck);
    }

    std::unordered_set<uint32_t> collectables_in_snapshot;
    for (const Gun& gun: last_snapshot.guns) {
        collectables_in_snapshot.insert(gun.gun_id);
        if (collectables_renderables.find(gun.gun_id) == collectables_renderables.end()) {
            collectables_renderables[gun.gun_id] =
                    std::make_unique<RenderableCollectable>(gun.gun_id, gun.type);
        }
        collectables_renderables[gun.gun_id]->update(gun);
    }

    for (auto it = collectables_renderables.begin(); it != collectables_renderables.end();) {
        if (collectables_in_snapshot.find(it->first) == collectables_in_snapshot.end()) {
            it = collectables_renderables.erase(it);  // al ser smart pointers se libera la memoria
        } else {
            ++it;
        }
    }

    std::unordered_set<uint32_t> bullets_in_snapshot;
    for (const Bullet& bullet: last_snapshot.bullets) {
        bullets_in_snapshot.insert(bullet.bullet_id);
        if (bullets_renderables.find(bullet.bullet_id) == bullets_renderables.end()) {
            if (bullet.type == Helmet || bullet.type == Armor)
                continue;
            sound_manager.shoot_sound(bullet.type);
            bullets_renderables[bullet.bullet_id] =
                    std::make_unique<RenderableBullet>(bullet.bullet_id, bullet.type);
        }
        bullets_renderables[bullet.bullet_id]->update(bullet);
    }

    for (auto it = bullets_renderables.begin(); it != bullets_renderables.end();) {
        if (bullets_in_snapshot.find(it->first) == bullets_in_snapshot.end()) {
            it = bullets_renderables.erase(it);
        } else {
            ++it;
        }
    }
}

void ConstantLooper::render(Camera& camera, RenderableMap& map) {
    renderer.Clear();

    map.render(renderer, camera);

    for (auto& bullet: bullets_renderables) {
        bullet.second->render(renderer, camera);
    }

    for (auto& duck: ducks_renderables) {
        if (duck.first == duck_id)
            continue;
        duck.second->render(renderer, camera);
    }
    ducks_renderables[duck_id]->render(renderer, camera);

    for (auto& collectable: collectables_renderables) {
        collectable.second->render(renderer, camera);
    }

    renderer.Present();
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

        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_2) {
            window.SetFullscreen(!(window.GetFlags() & SDL_WINDOW_FULLSCREEN));
        }

        p1_controller.process_event(event);
        // para cuando haya un segundo jugador
        // p2_controller.process_event(event);
    }

    p1_controller.send_last_move_command();
    // p2_controller.send_last_move_command();
    return true;
}

void ConstantLooper::clear_renderables() {
    // al ser smart pointers se libera la memoria
    ducks_renderables.clear();
    collectables_renderables.clear();
    bullets_renderables.clear();
}

ConstantLooper::~ConstantLooper() { clear_renderables(); }

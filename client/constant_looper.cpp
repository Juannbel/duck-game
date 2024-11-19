#include "constant_looper.h"

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>

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
#include "common/blocking_queue.h"
#include "common/config.h"
#include "common/lobby.h"
#include "common/map_dto.h"
#include "common/snapshot.h"

#include "animation_data_provider.h"
#include "controls_config.h"

#define USE_CAMERA true

static Config& config = Config::get_instance();

const static int RATE = 1000 / config.get_client_fps();
const static std::string WIN_TITLE = config.get_window_title();
const static int WIN_WIDTH = config.get_window_width();
const static int WIN_HEIGHT = config.get_window_height();

ConstantLooper::ConstantLooper(std::pair<uint8_t, uint8_t> duck_ids, Queue<Snapshot>& snapshot_q,
                               Queue<action>& actions_q):
        duck_ids(duck_ids),
        sdl(SDL_INIT_VIDEO | SDL_INIT_AUDIO),
        window(WIN_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIN_WIDTH, WIN_HEIGHT,
               SDL_WINDOW_RESIZABLE),
        renderer(window, -1, SDL_RENDERER_ACCELERATED),
        screen_manager(renderer, duck_ids),
        snapshot_q(snapshot_q),
        actions_q(actions_q),
        p1_controller(duck_ids.first, actions_q, last_snapshot, P1_CONTROLS),
        p2_controller(duck_ids.second, actions_q, last_snapshot, P2_CONTROLS),
        map_dto() {}

void ConstantLooper::run() try {
    TexturesProvider::load_textures(renderer);
    AnimationDataProvider::load_animations_data();

    if (!screen_manager.waiting_screen(snapshot_q, last_snapshot))
        return;

    process_snapshot();

    Camera camera(renderer);

    RenderableMap map(map_dto);

    if (!screen_manager.initial_screen(snapshot_q, last_snapshot, map, camera))
        return;

    bool keep_running = true;
    if (last_snapshot.game_finished) {
        // partida no iniciada
        screen_manager.between_rounds_screen(snapshot_q, last_snapshot, map, camera);
        return;
    }

    while (keep_running) {
        uint32_t t1 = SDL_GetTicks();

        keep_running = process_events();

        while (snapshot_q.try_pop(last_snapshot)) {}

        if (last_snapshot.round_finished || last_snapshot.game_finished) {
            keep_running =
                    screen_manager.between_rounds_screen(snapshot_q, last_snapshot, map, camera);
            if (!keep_running)
                continue;
            clear_renderables();
            process_snapshot();
            map.update(map_dto);
            camera.update(last_snapshot);
            p1_controller.restart_movement();
            p2_controller.restart_movement();
            continue;
        }

        // Actualizar el estado de todo lo que se renderiza
        process_snapshot();

        if (USE_CAMERA) {
            camera.update(last_snapshot);
        }

        render(camera, map);

        sound_manager.update();

        sleep_or_catch_up(t1);
    }
} catch (const ClosedQueue& e) {
    // El servidor se desconecto sin enviar ultimas snapshots
}

void ConstantLooper::sleep_or_catch_up(uint32_t& t1) {
    uint32_t t2 = SDL_GetTicks();

    int rest = RATE - (t2 - t1);
    if (rest < 0) {
        int behind = -rest;
        int lost = behind - behind % RATE;

        // recuperamos los frames perdidos
        uint8_t frames_to_skip = int(lost / RATE);

        for (auto& duck: ducks_renderables) {
            duck.second->skip_frames(frames_to_skip);
        }

        t1 += lost;
    } else {
        SDL_Delay(rest);
    }

    t1 = SDL_GetTicks();
}

void ConstantLooper::process_snapshot() {
    if (!last_snapshot.maps.empty()) {
        map_dto = last_snapshot.maps[0];
    }

    std::unordered_set<uint8_t> ducks_in_snapshot;
    for (auto& duck: last_snapshot.ducks) {
        ducks_in_snapshot.insert(duck.duck_id);
        if (ducks_renderables.find(duck.duck_id) != ducks_renderables.end()) {
            if (!ducks_renderables[duck.duck_id]->is_dead() && duck.is_dead)
                sound_manager.dead_sound();
            if (duck.gun == ActiveGrenade)
                sound_manager.active_grenade_sound();
        }
        ducks_renderables.try_emplace(duck.duck_id, std::make_unique<RenderableDuck>(duck.duck_id));
        ducks_renderables[duck.duck_id]->update(duck);
    }

    for (auto it = ducks_renderables.begin(); it != ducks_renderables.end();) {
        if (ducks_in_snapshot.find(it->first) == ducks_in_snapshot.end()) {
            it = ducks_renderables.erase(it);
        } else {
            ++it;
        }
    }

    std::unordered_set<uint32_t> boxes_in_snapshot;
    for (const Box& box: last_snapshot.boxes) {
        boxes_in_snapshot.insert(box.box_id);
        boxes_renderables.try_emplace(box.box_id, std::make_unique<RenderableBox>(box.box_id));
        boxes_renderables[box.box_id]->update(box);
    }

    for (auto it = boxes_renderables.begin(); it != boxes_renderables.end();) {
        if (boxes_in_snapshot.find(it->first) == boxes_in_snapshot.end()) {
            it = boxes_renderables.erase(it);
        } else {
            ++it;
        }
    }

    std::unordered_set<uint32_t> collectables_in_snapshot;
    for (const Gun& gun: last_snapshot.guns) {
        collectables_in_snapshot.insert(gun.gun_id);
        collectables_renderables.try_emplace(
                gun.gun_id, std::make_unique<RenderableCollectable>(gun.gun_id, gun.type));
        if (gun.type == ActiveGrenade)
            sound_manager.active_grenade_sound();
        collectables_renderables[gun.gun_id]->update(gun);
    }

    for (auto it = collectables_renderables.begin(); it != collectables_renderables.end();) {
        if (collectables_in_snapshot.find(it->first) == collectables_in_snapshot.end()) {
            it = collectables_renderables.erase(it);
        } else {
            ++it;
        }
    }

    std::unordered_set<uint32_t> bullets_in_snapshot;
    for (const Bullet& bullet: last_snapshot.bullets) {
        bullets_in_snapshot.insert(bullet.bullet_id);
        if (bullets_renderables.find(bullet.bullet_id) == bullets_renderables.end())
            sound_manager.shoot_sound(bullet.type);

        bullets_renderables.try_emplace(bullet.bullet_id, std::make_unique<RenderableBullet>(
                                                                  bullet.bullet_id, bullet.type));
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

    for (auto& box: boxes_renderables) {
        box.second->render(renderer, camera);
    }

    for (auto& bullet: bullets_renderables) {
        bullet.second->render(renderer, camera);
    }

    for (auto& duck: ducks_renderables) {
        if (duck.first == duck_ids.first || duck.first == duck_ids.second)
            continue;
        duck.second->render(renderer, camera);
    }
    ducks_renderables[duck_ids.first]->render(renderer, camera);
    if (duck_ids.second != INVALID_DUCK_ID) {
        ducks_renderables[duck_ids.second]->render(renderer, camera);
    }

    for (auto& collectable: collectables_renderables) {
        collectable.second->render(renderer, camera);
    }

    renderer.Present();
}


bool ConstantLooper::process_events() {
    SDL_Event event;
    p1_controller.update_duck_status();
    if (duck_ids.second != INVALID_DUCK_ID)
        p2_controller.update_duck_status();

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
        if (duck_ids.second != INVALID_DUCK_ID)
            p2_controller.process_event(event);
    }

    p1_controller.send_last_move_command();
    if (duck_ids.second != INVALID_DUCK_ID)
        p2_controller.send_last_move_command();

    return true;
}

void ConstantLooper::clear_renderables() {
    // al ser smart pointers se libera la memoria
    ducks_renderables.clear();
    collectables_renderables.clear();
    bullets_renderables.clear();
    boxes_renderables.clear();
}

ConstantLooper::~ConstantLooper() { clear_renderables(); }

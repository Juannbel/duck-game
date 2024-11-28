#ifndef CONSTANT_LOOPER_H
#define CONSTANT_LOOPER_H

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <utility>

#include "SDL2pp/Renderer.hh"
#include "SDL2pp/SDL.hh"
#include "SDL2pp/Window.hh"
#include "client/camera.h"
#include "client/renderables/box.h"
#include "client/renderables/bullet.h"
#include "client/renderables/collectable.h"
#include "client/renderables/map.h"
#include "client/screen_manager.h"
#include "client/sound_manager.h"
#include "common/blocking_queue.h"
#include "common/commands.h"
#include "common/map_dto.h"
#include "common/snapshot.h"
#include "renderables/duck.h"

#include "duck_controller.h"

class ConstantLooper {
private:
    std::pair<uint8_t, uint8_t> duck_ids;
    SDL2pp::SDL sdl;
    SDL2pp::Window window;
    SDL2pp::Renderer renderer;
    SoundManager sound_manager;
    Queue<Snapshot>& snapshot_q;
    Queue<action>& actions_q;
    Snapshot last_snapshot;
    DuckController p1_controller;
    DuckController p2_controller;
    bool play_again;
    MapDto map_dto;
    Camera camera;
    RenderableMap map;
    ScreenManager screen_manager;
    bool is_first_round;

    std::unordered_map<uint8_t, std::unique_ptr<RenderableDuck>> ducks_renderables;
    std::unordered_map<uint32_t, std::unique_ptr<RenderableBox>> boxes_renderables;
    std::unordered_map<uint32_t, std::unique_ptr<RenderableCollectable>> collectables_renderables;
    std::unordered_map<uint32_t, std::unique_ptr<RenderableBullet>> bullets_renderables;

    bool process_events();

    void update_ducks();
    void update_boxes();
    void update_collectables();
    void update_bullets();

    void process_snapshot();

    void render(Camera& camera, RenderableMap& map);

    void sleep_or_catch_up(uint32_t& t1);

    SDL2pp::Rect get_minimum_bounding_box();

    void clear_renderables();

public:
    ConstantLooper(std::pair<uint8_t, uint8_t> duck_ids, Queue<Snapshot>& snapshot_q,
                   Queue<action>& actions_q);

    bool run();

    ~ConstantLooper();
};

#endif

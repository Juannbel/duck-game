#ifndef CONSTANT_LOOPER_H
#define CONSTANT_LOOPER_H

#include <unordered_map>

#include "SDL2pp/Renderer.hh"
#include "client/renderables/bullet.h"
#include "client/renderables/collectable.h"
#include "client/renderables/map.h"
#include "client/sound_manager.h"
#include "common/blocking_queue.h"
#include "common/commands.h"
#include "common/map_dto.h"
#include "common/snapshot.h"
#include "duck_controller.h"
#include "renderables/duck.h"

class ConstantLooper {
private:
    uint8_t duck_id;
    SoundManager sound_manager;
    Queue<Snapshot>& snapshot_q;
    Queue<Command>& command_q;
    Snapshot last_snapshot;
    DuckController p1_controller;
    Map map_dto;

    std::unordered_map<uint8_t, RenderableDuck*> ducks_renderables;
    std::unordered_map<uint32_t, RenderableCollectable*> collectables_renderables;
    std::unordered_map<uint32_t, RenderableBullet*> bullets_renderables;

    bool process_events();

    void process_snapshot();

    bool waiting_screen(SDL2pp::Renderer& renderer);

    void render(SDL2pp::Renderer& renderer, Camera& camera, RenderableMap& map);

    void sleep_or_catch_up(uint32_t& t1);

    SDL2pp::Rect get_minimum_bounding_box();

public:
    ConstantLooper(MatchInfo& match_info, Queue<Snapshot>& snapshot_q, Queue<Command>& command_q);

    void run();

    ~ConstantLooper();
};

#endif

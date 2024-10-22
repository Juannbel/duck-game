#ifndef CONSTANT_LOOPER_H
#define CONSTANT_LOOPER_H

#include <unordered_map>

#include "../common/blocking_queue.h"
#include "../common/commands.h"
#include "../common/snapshot.h"
#include "client/renderables/duck.h"
#include "client/renderables/equipped_gun.h"

// procesa los eventos de teclado
// crea el Renderer, y que guarde las texturas de los sprites, y vaya generando los renderables

class ConstantLooper {
private:
    uint8_t duck_id;
    Queue<Snapshot>& snapshot_q;
    Queue<Command>& command_q;

    std::unordered_map<uint8_t, RenderableDuck*> ducks_renderables;

    bool process_events(const Snapshot& last_snapshot);

    void process_snapshot(const Snapshot& last_snapshot);

    SDL2pp::Rect get_minimum_bounding_box();

public:
    ConstantLooper(uint8_t duck_id, Queue<Snapshot>& snapshot_q, Queue<Command>& command_q);

    void run();

    ~ConstantLooper();
};

#endif

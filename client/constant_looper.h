#ifndef CONSTANT_LOOPER_H
#define CONSTANT_LOOPER_H

#include "../common/commands.h"
#include "../common/snapshot.h"
#include "../common/blocking_queue.h"
#include "client/renderables/duck.h"
#include "client/renderables/equipped_gun.h"
#include <unordered_map>

// procesa los eventos de teclado
// crea el Renderer, y que guarde las texturas de los sprites, y vaya generando los renderables

class ConstantLooper {
    private:
        uint8_t duck_id;
        Queue<Snapshot> &snapshot_q;
        Queue<Command> &command_q;

        std::unordered_map<uint8_t, RenderableDuck*> ducks_renderables;

        bool process_events(Snapshot &last_snapshot);

        void process_snapshot(Snapshot &last_snapshot);

        Rect get_minimum_bounding_box();

    public:
        ConstantLooper(uint8_t duck_id, Queue<Snapshot> &snapshot_q, Queue<Command> &command_q);

        void run();

        ~ConstantLooper();
};

#endif
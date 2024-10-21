#ifndef CONSTANT_LOOPER_H
#define CONSTANT_LOOPER_H

#include "../common/commands.h"
#include "../common/snapshot.h"
#include "client/renderables/duck.h"
#include "client/renderables/equipped_gun.h"
#include <unordered_map>

// procesa los eventos de teclado
// crea el Renderer, y que guarde las texturas de los sprites, y vaya generando los renderables

class ConstantLooper {
    private:
        // cola unica para mandar los comandos
        // Queue<Command>& commands;
        // cola para recibir los snapshots
        // Queue<Snapshot>& snapshots;
        uint8_t duck_id;
        Snapshot& last_snapshot;

        std::unordered_map<uint8_t, RenderableDuck*> ducks_renderables;

        bool process_events();

        void process_snapshot();

        Rect get_minimum_bounding_box();

    public:
        ConstantLooper(uint8_t duck_id, Snapshot& snapshot);

        void run();
};

#endif
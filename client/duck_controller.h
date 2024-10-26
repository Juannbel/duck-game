#ifndef DUCK_CONTROLLER_H
#define DUCK_CONTROLLER_H

#include "common/blocking_queue.h"
#include "common/commands.h"
#include "common/snapshot.h"

#include <SDL2/SDL.h>
#include <SDL_events.h>

struct ControlScheme {
    SDL_Keycode move_right;
    SDL_Keycode move_left;
    SDL_Keycode jump;
    SDL_Keycode lay_down;
    SDL_Keycode pick_up;
    SDL_Keycode shoot;
    SDL_Keycode look_up;
};

class DuckController {
private:
    int duck_id;
    Queue<Command>& command_q;  // Cola de comandos compartida
    const Snapshot& snapshot;
    ControlScheme controls;

public:
    DuckController(int duck_id, Queue<Command>& command_q, const Snapshot& snapshot, ControlScheme controls);

    void handle_key_down(const SDL_Event& event);

    void handle_key_up(const SDL_Event& event);

    bool process_events();

};

#endif

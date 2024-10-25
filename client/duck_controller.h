#ifndef DUCK_CONTROLLER_H
#define DUCK_CONTROLLER_H

#include "common/blocking_queue.h"
#include "common/commands.h"
#include "common/snapshot.h"

#include <SDL2/SDL.h>

struct ControlScheme {
    SDL_Keycode move_right;
    SDL_Keycode move_left;
    SDL_Keycode jump;
    SDL_Keycode lay_down;
    SDL_Keycode pick_up;
    SDL_Keycode shoot;
};


class DuckController {
private:
    int duck_id;
    Queue<Command>& command_q;  // Cola de comandos compartida
    const Snapshot& snapshot;
    ControlScheme controls;

public:
    DuckController(int duck_id, Queue<Command>& command_q, const Snapshot& snapshot, ControlScheme controls)
        : duck_id(duck_id), command_q(command_q), snapshot(snapshot), controls(controls) {}

    void handleKeyDown(const SDL_Event& event) {
        if (snapshot.ducks[duck_id].is_dead) return;

        if (event.key.keysym.sym == controls.move_right) {
            if (!snapshot.ducks[duck_id].is_running || !snapshot.ducks[duck_id].facing_right) {
                command_q.push(StartMovingRight);
            }
        } else if (event.key.keysym.sym == controls.move_left) {
            if (!snapshot.ducks[duck_id].is_running || snapshot.ducks[duck_id].facing_right) {
                command_q.push(StartMovingLeft);
            }
        } else if (event.key.keysym.sym == controls.jump) {
            command_q.push(Jump);
        } else if (event.key.keysym.sym == controls.lay_down) {
            command_q.push(LayDown);
        } else if (event.key.keysym.sym == controls.shoot) {
            command_q.push(StartShooting);
        } else if (event.key.keysym.sym == controls.pick_up) {
            command_q.push(PickUp);
        }
    }

    void handleKeyUp(const SDL_Event& event) {
        if (event.key.keysym.sym == controls.move_right) {
            if (snapshot.ducks[duck_id].is_running && snapshot.ducks[duck_id].facing_right) {
                command_q.push(StopMoving);
            }
        } else if (event.key.keysym.sym == controls.move_left) {
            if (snapshot.ducks[duck_id].is_running && !snapshot.ducks[duck_id].facing_right) {
                command_q.push(StopMoving);
            }
        } else if (event.key.keysym.sym == controls.lay_down) {
            if (snapshot.ducks[duck_id].is_laying) {
                command_q.push(StandUp);
            }
        } else if (event.key.keysym.sym == controls.shoot) {
            command_q.push(StopShooting);
        }
    }

    bool process_events() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT ||
                (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                return false;
            }

            if (snapshot.ducks[duck_id].is_dead) return true;

            if (event.type == SDL_KEYDOWN) {
                handleKeyDown(event);
            } else if (event.type == SDL_KEYUP) {
                handleKeyUp(event);
            }
        }
        return true;
    }
};

#endif

#include "duck_controller.h"

DuckController::DuckController(int duck_id, Queue<Command>& command_q, const Snapshot& snapshot, ControlScheme controls)
: duck_id(duck_id), command_q(command_q), snapshot(snapshot), controls(controls) {}

void DuckController::handle_key_down(const SDL_Event& event) {
    if (event.key.keysym.sym == controls.move_right) {
        if (!snapshot.ducks[duck_id].is_running || !snapshot.ducks[duck_id].facing_right)
            command_q.push(StartMovingRight);
    } else if (event.key.keysym.sym == controls.move_left) {
        if (!snapshot.ducks[duck_id].is_running || snapshot.ducks[duck_id].facing_right)
            command_q.push(StartMovingLeft);
    } else if (event.key.keysym.sym == controls.jump) {
        command_q.push(Jump);
    } else if (event.key.keysym.sym == controls.lay_down) {
        if (!snapshot.ducks[duck_id].is_laying)
            command_q.push(LayDown);
    } else if (event.key.keysym.sym == controls.shoot) {
        command_q.push(StartShooting);
    } else if (event.key.keysym.sym == controls.pick_up) {
        command_q.push(PickUp);
    } else if (event.key.keysym.sym == controls.look_up) {
        if (!snapshot.ducks[duck_id].facing_up)
            command_q.push(StartLookup);
    }
}

void DuckController::handle_key_up(const SDL_Event& event) {
    if (event.key.keysym.sym == controls.move_right) {
        command_q.push(StopMoving);
    } else if (event.key.keysym.sym == controls.move_left) {
        command_q.push(StopMoving);
    } else if (event.key.keysym.sym == controls.lay_down) {
        command_q.push(StandUp);
    } else if (event.key.keysym.sym == controls.shoot) {
        command_q.push(StopShooting);
    } else if (event.key.keysym.sym == controls.look_up) {
        command_q.push(StopLookup);
    }
}

bool DuckController::process_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT ||
            (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
            return false;
        }

        if (snapshot.ducks[duck_id].is_dead) return true;

        if (event.type == SDL_KEYDOWN) {
            handle_key_down(event);
        } else if (event.type == SDL_KEYUP) {
            handle_key_up(event);
        }
    }
    return true;
}

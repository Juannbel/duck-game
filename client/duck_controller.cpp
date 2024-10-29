#include "duck_controller.h"

DuckController::DuckController(int duck_id, Queue<Command>& command_q, const Snapshot& snapshot,
                               ControlScheme controls):
        duck_id(duck_id),
        command_q(command_q),
        snapshot(snapshot),
        controls(controls),
        move_command(false),
        moving_left(false),
        moving_right(false) {}

void DuckController::handle_key_down(const SDL_Event& event) {
    if (event.key.keysym.sym == controls.move_right) {
        if (!moving_right &&
            !(snapshot.ducks[duck_id].is_running && snapshot.ducks[duck_id].facing_right)) {
            moving_right = true;
            last_move_command = StartMovingRight;
            move_command = true;
        }
    } else if (event.key.keysym.sym == controls.move_left) {
        if (!moving_left &&
            !(snapshot.ducks[duck_id].is_running && !snapshot.ducks[duck_id].facing_right)) {
            moving_left = true;
            last_move_command = StartMovingLeft;
            move_command = true;
        }
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
        moving_right = false;
        if (moving_left) {
            last_move_command = StartMovingLeft;
            move_command = true;
        } else {
            last_move_command = StopMoving;
            move_command = true;
        }
    } else if (event.key.keysym.sym == controls.move_left) {
        moving_left = false;
        if (moving_right) {
            last_move_command = StartMovingRight;
            move_command = true;
        } else {
            last_move_command = StopMoving;
            move_command = true;
        }
    } else if (event.key.keysym.sym == controls.lay_down) {
        command_q.push(StandUp);
    } else if (event.key.keysym.sym == controls.shoot) {
        command_q.push(StopShooting);
    } else if (event.key.keysym.sym == controls.look_up) {
        command_q.push(StopLookup);
    } else if (event.key.keysym.sym == controls.jump) {
        command_q.push(StopJump);
    }
}

void DuckController::process_event(const SDL_Event& event) {
    if (snapshot.ducks[duck_id].is_dead)
        return;

    if (event.type == SDL_KEYDOWN) {
        handle_key_down(event);
    } else if (event.type == SDL_KEYUP) {
        handle_key_up(event);
    }
}

void DuckController::send_last_move_command() {
    if (move_command) {
        command_q.push(last_move_command);
        move_command = false;
    }
}

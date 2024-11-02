#include "duck_controller.h"

#include <algorithm>

DuckController::DuckController(int duck_id, Queue<Command>& command_q, Snapshot& snapshot,
                               ControlScheme controls):
        duck_id(duck_id),
        command_q(command_q),
        snapshot(snapshot),
        controls(controls),
        move_command(false),
        moving_left(false),
        moving_right(false) {}

void DuckController::handle_key_down(const SDL_Event& event, const Duck& duck) {
    if (event.key.keysym.sym == controls.move_right) {
        if (!moving_right &&
            !(duck.is_running && duck.facing_right)) {
            moving_right = true;
            last_move_command = StartMovingRight;
            move_command = true;
        }
    } else if (event.key.keysym.sym == controls.move_left) {
        if (!moving_left &&
            !(duck.is_running && !duck.facing_right)) {
            moving_left = true;
            last_move_command = StartMovingLeft;
            move_command = true;
        }
    } else if (event.key.keysym.sym == controls.jump) {
        command_q.push(Jump);
    } else if (event.key.keysym.sym == controls.lay_down) {
        if (!duck.is_laying)
            command_q.push(LayDown);
    } else if (event.key.keysym.sym == controls.shoot) {
        if (!duck.is_shooting)
            command_q.push(StartShooting);
    } else if (event.key.keysym.sym == controls.pick_up) {
        command_q.push(PickUp);
    } else if (event.key.keysym.sym == controls.look_up) {
        if (!duck.facing_up)
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
    auto it = std::find_if(snapshot.ducks.begin(), snapshot.ducks.end(),
                           [this](const Duck& duck) { return duck.duck_id == duck_id; });
    if (it == snapshot.ducks.end() || it->is_dead)
        return;

    const Duck& duck = *it;

    if (event.type == SDL_KEYDOWN) {
        handle_key_down(event, duck);
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

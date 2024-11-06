#include "duck_controller.h"

#include <algorithm>

DuckController::DuckController(uint8_t duck_id, Queue<action>& actions_q, Snapshot& snapshot,
                               ControlScheme controls):
        duck_id(duck_id),
        actions_q(actions_q),
        snapshot(snapshot),
        controls(controls),
        move_command(false),
        moving_left(false),
        moving_right(false) {}

void DuckController::handle_key_down(const SDL_Event& event, const Duck& duck) {
    if (event.key.keysym.sym == controls.move_right) {
        if (!moving_right && !(duck.is_running && duck.facing_right)) {
            moving_right = true;
            last_move_command = StartMovingRight;
            move_command = true;
        }
    } else if (event.key.keysym.sym == controls.move_left) {
        if (!moving_left && !(duck.is_running && !duck.facing_right)) {
            moving_left = true;
            last_move_command = StartMovingLeft;
            move_command = true;
        }
    } else if (event.key.keysym.sym == controls.jump) {
        actions_q.push({duck_id, Jump});
    } else if (event.key.keysym.sym == controls.lay_down) {
        if (!duck.is_laying)
            actions_q.push({duck_id, LayDown});
    } else if (event.key.keysym.sym == controls.shoot) {
        if (!duck.is_shooting)
            actions_q.push({duck_id, StartShooting});
    } else if (event.key.keysym.sym == controls.pick_up) {
        actions_q.push({duck_id, PickUp});
    } else if (event.key.keysym.sym == controls.look_up) {
        if (!duck.facing_up)
            actions_q.push({duck_id, StartLookup});
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
        actions_q.push({duck_id, StandUp});
    } else if (event.key.keysym.sym == controls.shoot) {
        actions_q.push({duck_id, StopShooting});
    } else if (event.key.keysym.sym == controls.look_up) {
        actions_q.push({duck_id, StopLookup});
    } else if (event.key.keysym.sym == controls.jump) {
        actions_q.push({duck_id, StopJump});
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
        actions_q.push({duck_id, last_move_command});
        move_command = false;
    }
}

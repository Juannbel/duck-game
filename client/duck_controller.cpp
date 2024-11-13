#include "duck_controller.h"

#include <algorithm>

#include "common/commands.h"
#include "common/lobby.h"

DuckController::DuckController(uint8_t duck_id, Queue<action>& actions_q, Snapshot& snapshot,
                               ControlScheme controls):
        duck_id(duck_id),
        actions_q(actions_q),
        snapshot(snapshot),
        controls(controls),
        move_command(false),
        moving_left(false),
        moving_right(false) {
    if (duck_id != INVALID_DUCK_ID)
        update_duck_status();
}

void DuckController::restart_movement() {
    moving_left = false;
    moving_right = false;
    move_command = false;
}

void DuckController::update_duck_status() {
    auto it = std::find_if(snapshot.ducks.begin(), snapshot.ducks.end(),
                           [this](const Duck& duck) { return duck.duck_id == duck_id; });
    if (it == snapshot.ducks.end() || it->is_dead)
        return;

    duck = *it;
}

void DuckController::handle_key_down(const SDL_Event& event) {
    const SDL_Keycode& key = event.key.keysym.sym;
    if (key == controls.move_right) {
        if (!moving_right && !(duck.is_running && duck.facing_right)) {
            moving_right = true;
            last_move_command = StartMovingRight;
            move_command = true;
        }
    } else if (key == controls.move_left) {
        if (!moving_left && !(duck.is_running && !duck.facing_right)) {
            moving_left = true;
            last_move_command = StartMovingLeft;
            move_command = true;
        }
    } else if (key == controls.jump) {
        actions_q.push({duck_id, Jump});
    } else if (key == controls.lay_down) {
        if (!duck.is_laying)
            actions_q.push({duck_id, LayDown});
    } else if (key == controls.shoot) {
        if (!duck.is_shooting)
            actions_q.push({duck_id, StartShooting});
    } else if (key == controls.pick_up) {
        actions_q.push({duck_id, PickUp});
    } else if (key == controls.look_up) {
        if (!duck.facing_up)
            actions_q.push({duck_id, StartLookup});
    } else if (key == controls.fly_mode) {
        actions_q.push({duck_id, FlyMode});
    } else if (key == controls.infinite_ammo) {
        actions_q.push({duck_id, InfiniteAmmo});
    } else if (key == controls.kill_everyone) {
        actions_q.push({duck_id, KillEveryone});
    } else if (key == controls.infinite_hp) {
        actions_q.push({duck_id, InfiniteHP});
    }
}

void DuckController::handle_key_up(const SDL_Event& event) {
    const SDL_Keycode& key = event.key.keysym.sym;
    if (key == controls.move_right) {
        moving_right = false;
        if (moving_left) {
            last_move_command = StartMovingLeft;
            move_command = true;
        } else {
            last_move_command = StopMoving;
            move_command = true;
        }
    } else if (key == controls.move_left) {
        moving_left = false;
        if (moving_right) {
            last_move_command = StartMovingRight;
            move_command = true;
        } else {
            last_move_command = StopMoving;
            move_command = true;
        }
    } else if (key == controls.lay_down) {
        actions_q.push({duck_id, StandUp});
    } else if (key == controls.shoot) {
        actions_q.push({duck_id, StopShooting});
    } else if (key == controls.look_up) {
        actions_q.push({duck_id, StopLookup});
    } else if (key == controls.jump) {
        actions_q.push({duck_id, StopJump});
    }
}

void DuckController::process_event(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        handle_key_down(event);
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

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

    key_down_handlers = {
            {controls.move_right, std::bind(&DuckController::handle_move_right, this)},
            {controls.move_left, std::bind(&DuckController::handle_move_left, this)},
            {controls.jump, std::bind(&DuckController::handle_jump, this)},
            {controls.lay_down, std::bind(&DuckController::handle_lay_down, this)},
            {controls.pick_up, std::bind(&DuckController::handle_pick_up, this)},
            {controls.shoot, std::bind(&DuckController::handle_shoot, this)},
            {controls.look_up, std::bind(&DuckController::handle_look_up, this)},
            {controls.fly_mode, std::bind(&DuckController::handle_fly_mode, this)},
            {controls.infinite_ammo, std::bind(&DuckController::handle_infinite_ammo, this)},
            {controls.kill_everyone, std::bind(&DuckController::handle_kill_everyone, this)},
            {controls.infinite_hp, std::bind(&DuckController::handle_infinite_hp, this)},
            {controls.get_death_laser, std::bind(&DuckController::handle_get_death_laser, this)}};

    key_up_handlers = {
            {controls.move_right, std::bind(&DuckController::handle_stop_move_right, this)},
            {controls.move_left, std::bind(&DuckController::handle_stop_move_left, this)},
            {controls.jump, std::bind(&DuckController::handle_stop_jump, this)},
            {controls.lay_down, std::bind(&DuckController::handle_stop_lay_down, this)},
            {controls.shoot, std::bind(&DuckController::handle_stop_shoot, this)},
            {controls.look_up, std::bind(&DuckController::handle_stop_look_up, this)}};
}

void DuckController::process_event(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        handle_key_down(event);
    } else if (event.type == SDL_KEYUP) {
        handle_key_up(event);
    }
}

void DuckController::handle_key_down(const SDL_Event& event) {
    const SDL_Keycode& key = event.key.keysym.sym;
    if (key_down_handlers.find(key) != key_down_handlers.end()) {
        key_down_handlers[key]();
    }
}

void DuckController::handle_key_up(const SDL_Event& event) {
    const SDL_Keycode& key = event.key.keysym.sym;
    if (key_up_handlers.find(key) != key_up_handlers.end()) {
        key_up_handlers[key]();
    }
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

void DuckController::send_last_move_command() {
    if (move_command) {
        actions_q.push({duck_id, last_move_command});
        move_command = false;
    }
}

// Handlers para key down

void DuckController::handle_move_right() {
    if (!moving_right && !(duck.is_running && duck.facing_right)) {
        moving_right = true;
        last_move_command = StartMovingRight;
        move_command = true;
    }
}

void DuckController::handle_move_left() {
    if (!moving_left && !(duck.is_running && !duck.facing_right)) {
        moving_left = true;
        last_move_command = StartMovingLeft;
        move_command = true;
    }
}

void DuckController::handle_jump() { actions_q.push({duck_id, Jump}); }

void DuckController::handle_lay_down() { actions_q.push({duck_id, LayDown}); }

void DuckController::handle_shoot() {
    if (!duck.is_shooting)
        actions_q.push({duck_id, StartShooting});
}

void DuckController::handle_look_up() {
    if (!duck.facing_up)
        actions_q.push({duck_id, StartLookup});
}

void DuckController::handle_pick_up() { actions_q.push({duck_id, PickUp}); }

void DuckController::handle_fly_mode() { actions_q.push({duck_id, FlyMode}); }

void DuckController::handle_infinite_ammo() { actions_q.push({duck_id, InfiniteAmmo}); }

void DuckController::handle_kill_everyone() { actions_q.push({duck_id, KillEveryone}); }

void DuckController::handle_infinite_hp() { actions_q.push({duck_id, InfiniteHP}); }

void DuckController::handle_get_death_laser() { actions_q.push({duck_id, GetDeathLaser}); }

// Handlers para key up

void DuckController::handle_stop_move_right() {
    moving_right = false;
    if (moving_left) {
        last_move_command = StartMovingLeft;
        move_command = true;
    } else {
        last_move_command = StopMoving;
        move_command = true;
    }
}

void DuckController::handle_stop_move_left() {
    moving_left = false;
    if (moving_right) {
        last_move_command = StartMovingRight;
        move_command = true;
    } else {
        last_move_command = StopMoving;
        move_command = true;
    }
}

void DuckController::handle_stop_jump() { actions_q.push({duck_id, StopJump}); }

void DuckController::handle_stop_lay_down() { actions_q.push({duck_id, StandUp}); }

void DuckController::handle_stop_shoot() { actions_q.push({duck_id, StopShooting}); }

void DuckController::handle_stop_look_up() { actions_q.push({duck_id, StopLookup}); }

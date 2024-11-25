#ifndef DUCK_CONTROLLER_H
#define DUCK_CONTROLLER_H

#include <functional>
#include <map>
#include <SDL2/SDL.h>
#include <SDL_events.h>

#include "common/blocking_queue.h"
#include "common/commands.h"
#include "common/snapshot.h"

struct ControlScheme {
    SDL_Keycode move_right;
    SDL_Keycode move_left;
    SDL_Keycode jump;
    SDL_Keycode lay_down;
    SDL_Keycode pick_up;
    SDL_Keycode shoot;
    SDL_Keycode look_up;

    // cheats
    SDL_KeyCode fly_mode;
    SDL_KeyCode infinite_ammo;
    SDL_KeyCode kill_everyone;
    SDL_KeyCode infinite_hp;
    SDL_KeyCode get_death_laser;
};

class DuckController {
private:
    uint8_t duck_id;
    Duck duck;
    Queue<action>& actions_q;
    Snapshot& snapshot;
    ControlScheme controls;

    Command last_move_command;
    bool move_command;
    bool moving_left;
    bool moving_right;

    std::map<SDL_Keycode, std::function<void()>> key_down_handlers;
    std::map<SDL_Keycode, std::function<void()>> key_up_handlers;

    void handle_key_down(const SDL_Event& event);
    void handle_key_up(const SDL_Event& event);

    void handle_move_right();
    void handle_move_left();
    void handle_jump();
    void handle_lay_down();
    void handle_shoot();
    void handle_pick_up();
    void handle_look_up();
    void handle_fly_mode();
    void handle_infinite_ammo();
    void handle_kill_everyone();
    void handle_infinite_hp();
    void handle_get_death_laser();

    void handle_stop_move_right();
    void handle_stop_move_left();
    void handle_stop_jump();
    void handle_stop_lay_down();
    void handle_stop_shoot();
    void handle_stop_look_up();

public:
    DuckController(uint8_t duck_id, Queue<action>& actions_q, Snapshot& snapshot,
                   ControlScheme controls);

    void update_duck_status();

    void process_event(const SDL_Event& event);

    void send_last_move_command();

    void restart_movement();
};

#endif

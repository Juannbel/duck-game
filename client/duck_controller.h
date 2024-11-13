#ifndef DUCK_CONTROLLER_H
#define DUCK_CONTROLLER_H

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

    void handle_key_down(const SDL_Event& event);
    void handle_key_up(const SDL_Event& event);

public:
    DuckController(uint8_t duck_id, Queue<action>& actions_q, Snapshot& snapshot,
                   ControlScheme controls);

    void update_duck_status();

    void process_event(const SDL_Event& event);

    void send_last_move_command();

    void restart_movement();
};

#endif

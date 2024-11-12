#ifndef CLIENT_CONFIG_H
#define CLIENT_CONFIG_H

#include <SDL_keycode.h>

#include "client/duck_controller.h"

#define FPS 120
#define RATE 1000 / FPS
#define WIN_WIDTH 1200
#define WIN_HEIGHT 690
#define WIN_TITLE "Duck game"

const ControlScheme P1_CONTROLS = {
        .move_right = SDLK_d,
        .move_left = SDLK_a,
        .jump = SDLK_w,
        .lay_down = SDLK_s,
        .pick_up = SDLK_c,
        .shoot = SDLK_v,
        .look_up = SDLK_e,
};

const ControlScheme P2_CONTROLS = {
        .move_right = SDLK_RIGHT,
        .move_left = SDLK_LEFT,
        .jump = SDLK_UP,
        .lay_down = SDLK_DOWN,
        .pick_up = SDLK_i,
        .shoot = SDLK_o,
        .look_up = SDLK_p,
};

#endif

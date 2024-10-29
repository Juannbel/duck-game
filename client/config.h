#ifndef CLIENT_CONFIG_H
#define CLIENT_CONFIG_H

#include "client/duck_controller.h"

#define FPS 60
#define RATE 1000/FPS
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

#endif

#ifndef CONTROLS_CONFIG_H
#define CONTROLS_CONFIG_H

#include <SDL_keycode.h>

#include "client/duck_controller.h"

const ControlScheme P1_CONTROLS = {.move_right = SDLK_d,
                                   .move_left = SDLK_a,
                                   .jump = SDLK_w,
                                   .lay_down = SDLK_s,
                                   .pick_up = SDLK_c,
                                   .shoot = SDLK_v,
                                   .look_up = SDLK_e,
                                   .fly_mode = SDLK_F1,
                                   .infinite_ammo = SDLK_F2,
                                   .kill_everyone = SDLK_F3,
                                   .infinite_hp = SDLK_F4,
                                   .get_death_laser = SDLK_F5,
                                   .heal_and_revive = SDLK_F6};

const ControlScheme P2_CONTROLS = {.move_right = SDLK_RIGHT,
                                   .move_left = SDLK_LEFT,
                                   .jump = SDLK_UP,
                                   .lay_down = SDLK_DOWN,
                                   .pick_up = SDLK_k,
                                   .shoot = SDLK_l,
                                   .look_up = SDLK_RSHIFT,
                                   .fly_mode = SDLK_F7,
                                   .infinite_ammo = SDLK_F8,
                                   .kill_everyone = SDLK_F9,
                                   .infinite_hp = SDLK_F10,
                                   .get_death_laser = SDLK_F11,
                                   .heal_and_revive = SDLK_F12};

#endif

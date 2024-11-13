#ifndef COMMANDS_H
#define COMMANDS_H

#include <cstdint>
enum Command {
    StartMovingRight,
    StartMovingLeft,
    StopMoving,
    StartShooting,
    StopShooting,
    StartLookup,
    StopLookup,
    PickUp,  // si tiene algo agarrado, es soltar
    Jump,    // si estaba saltando se interpreta como flapping
    StopJump,
    LayDown,
    StandUp,
    FlyMode,
    InfiniteAmmo,
    KillEveryone,
    InfiniteHP
};

struct action {
    uint8_t duck_id;  // del 0 al 3
    enum Command command;
} __attribute__((packed));

#endif

#ifndef COMMANDS_H
#define COMMANDS_H

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
};

#endif

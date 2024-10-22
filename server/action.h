#ifndef ACTION_H
#define ACTION_H

#include <cstdint>

#include "common/commands.h"

struct action {
    uint8_t duck_id;  // del 0 al 3
    enum Command command;
};


#endif

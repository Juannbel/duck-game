#ifndef LOBBY_MESSAGE_H
#define LOBBY_MESSAGE_H

#include <cstdint>

#define INVALID_GAME_ID -1
#define INVALID_DUCK_ID 255
#define LIST_GAMES 1
#define CREATE_GAME 2
#define JOIN_GAME 3

struct GameInfo {
    int32_t game_id;
    uint8_t duck_id;
} __attribute__((packed));

#endif

#ifndef LOBBY_MESSAGE_H
#define LOBBY_MESSAGE_H

#include <cstdint>

#include "../common/snapshot.h"

#define INVALID_GAME_ID -1
#define INVALID_DUCK_ID 255
#define LIST_GAMES 1
#define CREATE_GAME 2
#define JOIN_GAME 3
#define START_GAME 4
#define GET_INFO 5

struct GameInfo {
    int32_t game_id;
    uint8_t duck_id_1;
    uint8_t duck_id_2;
} __attribute__((packed));

struct LobbyInfo {
    int32_t game_id;
    uint8_t connected_players;
    char creator[MAX_PLAYER_NAME];
} __attribute__((packed));

#endif

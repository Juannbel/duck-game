#ifndef MAP_DTO_H
#define MAP_DTO_H

#include <cstdint>
#include "shared_constants.h"

enum BlockType { Empty, Floor, Wall, Core };

struct Map {
    BlockType blocks[MAP_HEIGHT_BLOCKS][MAP_WIDTH_BLOCKS];
} __attribute__((packed));

struct MatchInfo {
    uint8_t duck_id;
    Map map;
} __attribute__((packed));

#endif

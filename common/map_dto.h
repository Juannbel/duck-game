#ifndef MAP_DTO_H
#define MAP_DTO_H

#include "shared_constants.h"

enum BlockType { Empty, Floor, Wall, Core };

struct Map {
    BlockType blocks[MAP_HEIGHT_BLOCKS][MAP_WIDTH_BLOCKS];
} __attribute__((packed));

#endif

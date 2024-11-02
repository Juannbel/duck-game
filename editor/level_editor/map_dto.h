#ifndef MAP_DTO_H
#define MAP_DTO_H

#include <cstdint>

#define MAP_THEMES 4

enum BlockType {
    Empty,
    Floor1,
    Floor2,
    Floor3,
    Base1,
    Base2,
    Base3,
    Platform1,
    Platform2,
    Platform3,
    Platform4,
    Wall,
    HalfFloor
};

struct Block {
    BlockType type;
    bool solid;
} __attribute__((packed));

#define MAP_HEIGHT_BLOCKS 20
#define MAP_WIDTH_BLOCKS 35

struct Map {
    Block blocks[MAP_HEIGHT_BLOCKS][MAP_WIDTH_BLOCKS];
} __attribute__((packed));

#endif

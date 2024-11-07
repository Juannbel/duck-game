#ifndef MAP_H
#define MAP_H

#include <utility>
#include <vector>

#include "common/map_dto.h"

struct Map {
    MapDto map_dto;
    // vector de pares de coordenadas (x, y) de los spawns
    std::vector<std::pair<int16_t, int16_t>> duck_spawns;
    std::vector<std::pair<int16_t, int16_t>> collectables_spawns;
    std::vector<std::pair<int16_t, int16_t>> boxes_spawns;
};

#endif

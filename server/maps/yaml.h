#ifndef YAML_H
#define YAML_H

#include <common/map_dto.h>
#include <yaml-cpp/yaml.h>
#include "server/game/entitys_manager.h"

class YAMLLoader {
public:
    // int loadMap(Map* map_blocks_info, EntityManager* entity_manager);
    Map loadMap(const std::string& path);
};

#endif //YAML_H

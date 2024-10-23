#ifndef YAML_H
#define YAML_H

#include <common/map_dto.h>
#include <yaml-cpp/yaml.h>
#include "server/entitys_manager.h"

class YAMLLoader {
public:
    int loadMap(Map* map_blocks_info, EntityManager* entity_manager);
};

#endif //YAML_H

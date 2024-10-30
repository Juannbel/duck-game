#ifndef YAML_H
#define YAML_H

#include <string>
#include <unordered_map>

#include <common/map_dto.h>
#include <yaml-cpp/yaml.h>


class YAMLLoader {
private:
    static std::unordered_map<std::string, BlockType> string_to_block;

public:
    // int loadMap(Map* map_blocks_info, EntityManager* entity_manager);
    Map loadMap(const std::string& path);
};

#endif  // YAML_H

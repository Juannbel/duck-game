#ifndef YAML_H
#define YAML_H

#include <string>
#include <unordered_map>
#include <vector>

#include <common/map_dto.h>
#include <yaml-cpp/yaml.h>


class MapLoader {
private:
    static std::unordered_map<std::string, BlockType> string_to_block;

public:
    // int loadMap(Map* map_blocks_info, EntityManager* entity_manager);
    std::vector<std::string> list_maps(const std::string& path_to_dir);
    Map loadMap(const std::string& path);
};

#endif  // YAML_H

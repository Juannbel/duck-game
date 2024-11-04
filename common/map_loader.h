#ifndef YAML_H
#define YAML_H

#include <string>
#include <vector>

#include "map_dto.h"
#include <yaml-cpp/yaml.h>


class MapLoader {
public:
    std::vector<std::string> list_maps(const std::string& path_to_dir);

    Map loadMap(const std::string& path);

    void saveMap(const std::string& path, const Map& map);
};

#endif  // YAML_H

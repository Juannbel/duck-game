#ifndef YAML_H
#define YAML_H

#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>

#include "map.h"


class MapLoader {
public:
    std::vector<std::string> list_maps(const std::string& path_to_dir);

    Map load_map(const std::string& path);

    void save_map(const std::string& path, const Map& map);
};

#endif  // YAML_H

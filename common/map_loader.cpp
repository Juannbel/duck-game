#include "map_loader.h"

#include <cstdint>
#include <filesystem>
#include <fstream>

#include <sys/types.h>

std::vector<std::string> MapLoader::list_maps(const std::string& path_to_dir) {
    std::vector<std::string> maps;
    std::transform(
            std::filesystem::directory_iterator(path_to_dir), std::filesystem::directory_iterator(),
            std::back_inserter(maps),
            [](const std::filesystem::directory_entry& map_name) { return map_name.path(); });

    return maps;
}

Map MapLoader::load_map(const std::string& path) {
    MapDto map_blocks_info;

    YAML::Node config = YAML::LoadFile(path);

    const int height = config["height"].as<int>();
    const int width = config["width"].as<int>();
    map_blocks_info.theme = config["theme"].as<uint8_t>();

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            map_blocks_info.blocks[i][j].type = Empty;
            map_blocks_info.blocks[i][j].solid = false;
        }
    }

    for (const auto& block: config["blocks"]) {
        const BlockType type = static_cast<BlockType>(block["type"].as<int>());
        bool solid = block["solid"].as<bool>();

        const int x = block["x"].as<int>();
        const int y = block["y"].as<int>();

        if (x < 0 || x >= width || y < 0 || y >= height) {
            throw std::invalid_argument("Block out of bounds");
        }

        map_blocks_info.blocks[y][x].type = type;
        map_blocks_info.blocks[y][x].solid = solid;
    }

    Map map;
    map.map_dto = map_blocks_info;

    for (const auto& spawn: config["duck_spawns"]) {
        map.duck_spawns.emplace_back(spawn["x"].as<int>(), spawn["y"].as<int>());
    }

    for (const auto& spawn: config["collectables_spawns"]) {
        map.collectables_spawns.emplace_back(spawn["x"].as<int>(), spawn["y"].as<int>());
    }

    for (const auto& spawn: config["boxes_spawns"]) {
        map.boxes_spawns.emplace_back(spawn["x"].as<int>(), spawn["y"].as<int>());
    }

    return map;
}

bool MapLoader::save_map(const std::string& path, const Map& map) {
    const MapDto& map_dto = map.map_dto;

    YAML::Node root;
    root["width"] = MAP_WIDTH_BLOCKS;
    root["height"] = MAP_HEIGHT_BLOCKS;
    root["theme"] = static_cast<int>(map_dto.theme);

    YAML::Node blocksNode = root["blocks"];

    for (int i = 0; i < MAP_HEIGHT_BLOCKS; ++i) {
        for (int j = 0; j < MAP_WIDTH_BLOCKS; ++j) {
            const Block& block = map_dto.blocks[i][j];
            if (block.type != Empty) {
                YAML::Node blockNode;
                blockNode["x"] = j;
                blockNode["y"] = i;
                blockNode["type"] = static_cast<int>(block.type);
                blockNode["solid"] = block.solid;

                blocksNode.push_back(blockNode);
            }
        }
    }

    YAML::Node duck_spawns = root["duck_spawns"];
    for (const auto& spawn: map.duck_spawns) {
        YAML::Node spawnNode;
        spawnNode["x"] = spawn.first;
        spawnNode["y"] = spawn.second;
        duck_spawns.push_back(spawnNode);
    }

    YAML::Node collectables_spawns = root["collectables_spawns"];
    for (const auto& spawn: map.collectables_spawns) {
        YAML::Node spawnNode;
        spawnNode["x"] = spawn.first;
        spawnNode["y"] = spawn.second;
        collectables_spawns.push_back(spawnNode);
    }

    YAML::Node boxes_spawns = root["boxes_spawns"];
    for (const auto& spawn: map.boxes_spawns) {
        YAML::Node spawnNode;
        spawnNode["x"] = spawn.first;
        spawnNode["y"] = spawn.second;
        boxes_spawns.push_back(spawnNode);
    }

    std::ofstream file(path);
    if (!file.is_open()) {
        return false;
    }
    file << root;
    if (file.fail()) {
        return false;
    }
    file.close();
    return true;
}

#include "map_loader.h"

#include <cstdint>
#include <filesystem>

#include <sys/types.h>
#include <fstream>

std::vector<std::string> MapLoader::list_maps(const std::string& path_to_dir) {
    std::vector<std::string> maps;
    for (auto& map_name: std::filesystem::directory_iterator(path_to_dir)) {
        maps.push_back(map_name.path());
    }
    return maps;
}

Map MapLoader::loadMap(const std::string& path) {
    Map map_blocks_info;


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

    for (const auto& block : config["blocks"]) {
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

    return map_blocks_info;
}

void MapLoader::saveMap(const std::string& path, const Map& map) {
    YAML::Node root;
    root["width"] = MAP_WIDTH_BLOCKS;
    root["height"] = MAP_HEIGHT_BLOCKS;
    root["theme"] = static_cast<int>(map.theme);

    YAML::Node blocksNode = root["blocks"];

    for (int i = 0; i < MAP_HEIGHT_BLOCKS; ++i) {
        for (int j = 0; j < MAP_WIDTH_BLOCKS; ++j) {
            const Block &block = map.blocks[i][j];
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

    std::ofstream file(path);
    file << root;
    file.close();
}
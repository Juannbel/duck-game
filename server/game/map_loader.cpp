#include "map_loader.h"

#include <cstdint>
#include <filesystem>
#include <iostream>

#include <sys/types.h>

std::unordered_map<std::string, BlockType> MapLoader::string_to_block{
        {"empty", Empty},          {"floor_1", Floor1},       {"floor_2", Floor2},
        {"floor_3", Floor3},       {"base_1", Base1},         {"base_2", Base2},
        {"base_3", Base3},         {"platform_1", Platform1}, {"platform_2", Platform2},
        {"platform_3", Platform3}, {"platform_4", Platform4}, {"wall", Wall},
        {"half_floor", HalfFloor}};

// BlockType parseBlockType(const std::string& typeStr) {
//     if (typeStr == "Floor")
//         return BlockType::Floor;
//     if (typeStr == "Wall")
//         return BlockType::Wall;
//     throw std::invalid_argument("Unknown block type");
// }

std::vector<std::string> MapLoader::list_maps(const std::string& path_to_dir) {
    std::vector<std::string> maps;
    for (auto& map_name: std::filesystem::directory_iterator(path_to_dir)) {
        maps.push_back(map_name.path());
    }
    return maps;
}

// int MapLoader::loadMap(Map* map_blocks_info, EntityManager* entity_manager) {
Map MapLoader::loadMap(const std::string& path) {
    Map map_blocks_info;


    YAML::Node config = YAML::LoadFile(path);

    // std::string nombre = config["name"].as<std::string>();

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
        const BlockType type = string_to_block[block["type"].as<std::string>()];
        bool solid = block["solid"].as<bool>();

        const int x = block["x"].as<int>();
        const int y = block["y"].as<int>();

        if (x < 0 || x >= width || y < 0 || y >= height) {
            throw std::invalid_argument("Block out of bounds");
        }

        // Set the block type and solid property
        map_blocks_info.blocks[y][x].type = type;
        map_blocks_info.blocks[y][x].solid = solid;
    }

    return map_blocks_info;
}

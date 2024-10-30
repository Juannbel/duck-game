#include "yaml.h"

std::unordered_map<std::string, BlockType> YAMLLoader::string_to_block {
    {"empty", Empty},
    {"floor_1", Floor1},
    {"floor_2", Floor2},
    {"floor_3", Floor3},
    {"base_1", Base1},
    {"base_2", Base2},
    {"base_3", Base3},
    {"platform_1", Platform1},
    {"platform_2", Platform2},
    {"platform_3", Platform3},
    {"platform_4", Platform4},
    {"wall", Wall},
    {"half_floor", HalfFloor}
};

// BlockType parseBlockType(const std::string& typeStr) {
//     if (typeStr == "Floor")
//         return BlockType::Floor;
//     if (typeStr == "Wall")
//         return BlockType::Wall;
//     throw std::invalid_argument("Unknown block type");
// }

// int YAMLLoader::loadMap(Map* map_blocks_info, EntityManager* entity_manager) {
Map YAMLLoader::loadMap(const std::string& path) {
    Map map_blocks_info;

    YAML::Node config = YAML::LoadFile(path);

    std::string nombre = config["name"].as<std::string>();

    // TODO: Crear la pantalla con estos valores
    const int height = config["height"].as<int>();
    const int width = config["width"].as<int>();

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            map_blocks_info.blocks[i][j].type = BlockType::Empty;
            map_blocks_info.blocks[i][j].solid = false;
        }
    }

    for (const auto& block: config["blocks"]) {
        const BlockType type = string_to_block[block["type"].as<std::string>()];
        bool solid = block["solid"].as<bool>();

        auto start = block["start"];
        const int x = start["x"].as<int>();
        const int y = start["y"].as<int>();

        const int length = block["length"].as<int>();

        if (x < 0 || x >= width || y < 0 || y >= height) {
            throw std::invalid_argument("Block out of bounds");
        }

        auto direction = block["direction"].as<std::string>();

        if (direction == "x") {
            for (int i = 0; i < length; i++) {
                if (x + i >= width) {
                    throw std::invalid_argument("Block out of bounds");
                }
                map_blocks_info.blocks[y][x + i].type = type;
                map_blocks_info.blocks[y][x + i].solid = solid;
            }
        } else if (direction == "y") {
            for (int i = 0; i < length; i++) {
                if (y + i >= height) {
                    throw std::invalid_argument("Block out of bounds");
                }
                map_blocks_info.blocks[y + i][x].type = type;
                map_blocks_info.blocks[y + i][x].solid = solid;
            }
        }
    }

    return map_blocks_info;
}

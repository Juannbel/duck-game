#include "animation_data_provider.h"

#include <vector>

#include <yaml-cpp/yaml.h>

#include "client/renderables/animation.h"
#include "common/config.h"
#include "common/map_dto.h"
#include "common/snapshot.h"

#define FPS_BASE 60  // para cuantos fps fueron diseñadas las animaciones

static Config& config = Config::get_instance();
const static int FPS = config.get_client_fps();

void AnimationDataProvider::load_animations_data() {
    for (uint8_t i = 0; i < MAX_DUCKS; i++) {
        load_from_yaml("duck_" + std::to_string(i),
                       DATA_PATH "/sprites/duck/duck_" + std::to_string(i) + ".yaml");
        load_from_yaml("wings_" + std::to_string(i),
                       DATA_PATH "/sprites/wings/wings_" + std::to_string(i) + ".yaml");
        load_from_yaml("feathers_" + std::to_string(i),
                       DATA_PATH "/sprites/feathers/feathers_" + std::to_string(i) + ".yaml");
    }


    for (uint8_t i = 0; i < MAP_THEMES; i++) {
        load_from_yaml("blocks_" + std::to_string(i),
                       DATA_PATH "/sprites/blocks/blocks_" + std::to_string(i) + ".yaml");
    }

    load_from_yaml("box", DATA_PATH "/sprites/boxes/boxes.yaml");
    load_from_yaml("collectables", DATA_PATH "/sprites/collectables/collectables.yaml");
    load_from_yaml("guns", DATA_PATH "/sprites/guns/guns.yaml");
    load_from_yaml("bullets", DATA_PATH "/sprites/bullets/bullets.yaml");
    load_from_yaml("helmet", DATA_PATH "/sprites/equipment/helmet.yaml");
    load_from_yaml("armor", DATA_PATH "/sprites/equipment/armor.yaml");
}

const AnimationData& AnimationDataProvider::get_animation_data(const std::string& name) {
    if (frames_data.find(name) == frames_data.end()) {
        throw std::runtime_error("Animation data not found: " + name);
    }
    return frames_data[name];
}

void AnimationDataProvider::load_from_yaml(const std::string& name, const std::string& path) {
    YAML::Node animation_config = YAML::LoadFile(path);

    int width = animation_config["width"].as<int>();
    int height = animation_config["height"].as<int>();

    for (const auto& obj: animation_config[name]) {
        std::string animation_name = obj.first.as<std::string>();
        std::vector<FrameData> frames;

        for (const auto& frame: obj.second["frames"]) {
            int source_x = frame["x"].as<int>();
            int source_y = frame["y"].as<int>();
            int x_offset_right = frame["x_offset_right"].as<int>();
            int x_offset_left = frame["x_offset_left"].as<int>();
            int y_offset = frame["y_offset"].as<int>();

            SDL2pp::Rect rect = SDL2pp::Rect(source_x, source_y, width, height);
            frames.push_back({rect, x_offset_right, x_offset_left, y_offset});
        }

        bool loops = obj.second["loops"].as<bool>();
        uint8_t iter_per_frame = obj.second["iter_per_frame"].as<uint8_t>();

        if (iter_per_frame != 1) {
            float fps_ratio = static_cast<float>(FPS) / FPS_BASE;
            iter_per_frame = static_cast<uint8_t>(iter_per_frame * fps_ratio);
        }

        frames_data[name + "_" + animation_name] = AnimationData{frames, iter_per_frame, loops};
    }
}

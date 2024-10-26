#include "animation_data_provider.h"
#include <yaml-cpp/yaml.h>
#include "client/renderables/animation.h"

std::unordered_map<std::string, AnimationData> AnimationDataProvider::frames_data;

void AnimationDataProvider::load_animations_data() {
    load_from_yaml("duck_0", DATA_PATH "/sprites/duck/frames_0.yaml");
    load_from_yaml("duck_1", DATA_PATH "/sprites/duck/frames_1.yaml");
    load_from_yaml("duck_2", DATA_PATH "/sprites/duck/frames_2.yaml");
    load_from_yaml("duck_3", DATA_PATH "/sprites/duck/frames_3.yaml");

    load_from_yaml("collectables", DATA_PATH "/sprites/collectables/collectables.yaml");
    load_from_yaml("guns", DATA_PATH "/sprites/guns/guns.yaml");
    load_from_yaml("helmet", DATA_PATH "/sprites/equipment/helmet.yaml");
}

const AnimationData& AnimationDataProvider::get_animation_data(const std::string& name) {
    return frames_data[name];
}

void AnimationDataProvider::load_from_yaml(const std::string& name, const std::string& path) {
    YAML::Node config = YAML::LoadFile(path);

    int width = config["width"].as<int>();
    int height = config["height"].as<int>();

    for (const auto& obj: config[name]) {
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

        uint8_t iter_per_frame = obj.second["iter_per_frame"].as<uint8_t>();
        bool loops = obj.second["loops"].as<bool>();

        frames_data[name + "_" + animation_name] = AnimationData{frames, iter_per_frame, loops};
    }
}

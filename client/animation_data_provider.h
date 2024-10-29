#ifndef ANIMATION_DATA_PROVIDER_H
#define ANIMATION_DATA_PROVIDER_H

#include <string>
#include <unordered_map>

#include "client/renderables/animation.h"

class AnimationDataProvider {
private:
    static std::unordered_map<std::string, AnimationData> frames_data;

    AnimationDataProvider() {}

    static void load_from_yaml(const std::string& name, const std::string& path);

public:
    static const AnimationData& get_animation_data(const std::string& name);

    static void load_animations_data();
};

#endif

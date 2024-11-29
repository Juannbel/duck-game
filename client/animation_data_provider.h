#ifndef ANIMATION_DATA_PROVIDER_H
#define ANIMATION_DATA_PROVIDER_H

#include <string>
#include <unordered_map>

#include "client/renderables/animation.h"

class AnimationDataProvider {
private:
    AnimationDataProvider() { load_animations_data(); }

    std::unordered_map<std::string, AnimationData> frames_data;

    void load_from_yaml(const std::string& name, const std::string& path);

public:
    static AnimationDataProvider& get_instance() {
        static AnimationDataProvider instance;
        return instance;
    }

    const AnimationData& get_animation_data(const std::string& name);

    void load_animations_data();
};

#endif

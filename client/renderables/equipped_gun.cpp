#include "equipped_gun.h"
#include <unordered_map>
#include "common/snapshot.h"

RenderableEquippedGun::RenderableEquippedGun(SDL2pp::Texture* sprite, const std::string& config_path):
        position(0, 0), facing_right(true), facing_up(false) {
    YAML::Node config = YAML::LoadFile(config_path);

    int width = config["width"].as<int>();
    int height = config["height"].as<int>();

    for (const auto& gun: config["guns"]) {
        GunType gun_type = string_to_gun[gun.first.as<std::string>()];

        std::vector<FrameData> frames;

        for (const auto& frame: gun.second["frames"]) {
            int source_x = frame["x"].as<int>();
            int source_y = frame["y"].as<int>();
            int x_offset_right = frame["x_offset_right"].as<int>();
            int x_offset_left = frame["x_offset_left"].as<int>();
            int y_offset = frame["y_offset"].as<int>();

            SDL2pp::Rect rect = SDL2pp::Rect(source_x, source_y, width, height);
            frames.push_back({rect, x_offset_right, x_offset_left, y_offset});
        }

        uint8_t iter_per_frame = gun.second["iter_per_frame"].as<uint8_t>();
        bool loops = gun.second["loops"].as<bool>();
        guns[gun_type] = new Animation(*sprite, frames, iter_per_frame, loops);
    }
    current_gun = None;
    curr_animation = guns[None];
}

std::unordered_map<std::string, GunType> RenderableEquippedGun::string_to_gun {
    {"none", None},
    {"grenade", Grenade},
    {"banana", Banana},
    {"pew_pew_laser", PewPewLaser},
    {"laser_rifle", LaserRifle},
    {"ak47", Ak47},
    {"dueling_pistol", DuelingPistol},
    {"cowboy_pistol", CowboyPistol},
    {"magnum", Magnum},
    {"shootgun", Shootgun},
    {"sniper", Sniper}
};

void RenderableEquippedGun::update() { curr_animation->update(); }

void RenderableEquippedGun::update_from_snapshot(const Duck& duck) {
    position.x = duck.x;
    position.y = duck.y;

    facing_right = duck.facing_right;
    facing_up = duck.facing_up;

    curr_animation = guns[duck.gun];
    current_gun = duck.gun;
}

void RenderableEquippedGun::render(SDL2pp::Renderer& renderer, Camera& camera) {
    if (current_gun == None) {
        return;
    }

    float angle = facing_up ? facing_right ? 290 : 70 : 0;
    curr_animation->render(renderer, camera, position, facing_right, angle);

}

RenderableEquippedGun::~RenderableEquippedGun() {
    for (auto& gun: guns) {
        delete gun.second;
    }
}

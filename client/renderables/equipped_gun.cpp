#include "equipped_gun.h"
#include <unordered_map>
#include "client/animation_data_provider.h"
#include "client/textures_provider.h"
#include "common/snapshot.h"

RenderableEquippedGun::RenderableEquippedGun() :
        position(0, 0), facing_right(true), facing_up(false) {

    for (auto& gun: string_to_gun) {
        load_gun_animation(gun.first);
    }
    current_gun = None;
    curr_animation = guns[None];
}

void RenderableEquippedGun::load_gun_animation(const std::string& gun_name) {
    guns[string_to_gun[gun_name]] = new Animation(
        *TexturesProvider::getTexture("guns"),
        AnimationDataProvider::get_animation_data("guns_" + gun_name)
    );
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

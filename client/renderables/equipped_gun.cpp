#include "equipped_gun.h"
#include <unordered_map>
#include "client/animation_data_provider.h"
#include "client/textures_provider.h"
#include "common/snapshot.h"

RenderableEquippedGun::RenderableEquippedGun() :
        position(0, 0), facing_right(true), facing_up(false) {

    for (auto& gun: gun_to_string) {
        load_gun_animation(gun.first);
    }
    current_gun = None;
    curr_animation = guns[None];
}

void RenderableEquippedGun::load_gun_animation(const GunType& gun) {
    guns[gun] = new Animation(
        *TexturesProvider::getTexture("guns"),
        AnimationDataProvider::get_animation_data("guns_" + gun_to_string[gun])
    );
}

std::unordered_map<GunType, std::string> RenderableEquippedGun::gun_to_string {
    {None, "none"},
    {Grenade, "grenade"},
    {Banana, "banana"},
    {PewPewLaser, "pew_pew_laser"},
    {LaserRifle, "laser_rifle"},
    {Ak47, "ak47"},
    {DuelingPistol, "dueling_pistol"},
    {CowboyPistol, "cowboy_pistol"},
    {Magnum, "magnum"},
    {Shootgun, "shootgun"},
    {Sniper, "sniper"},
    {Helmet, "helmet"},
    {Armor, "armor"}
};

void RenderableEquippedGun::update(const Duck& duck) {
    position.x = duck.x;
    position.y = duck.y;

    facing_right = duck.facing_right;
    facing_up = duck.facing_up;

    curr_animation = guns[duck.gun];
    current_gun = duck.gun;

    curr_animation->update();
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

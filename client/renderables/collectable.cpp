#include "collectable.h"

#include <unordered_map>

#include <yaml-cpp/yaml.h>

#include "client/animation_data_provider.h"
#include "client/textures_provider.h"
#include "common/shared_constants.h"
#include "common/snapshot.h"

RenderableCollectable::RenderableCollectable(uint32_t id, GunType type):
        id(id),
        animation(*TexturesProvider::get_texture("collectables"),
                  AnimationDataProvider::get_animation_data("collectables_" +
                                                            collectable_to_string[type])),
        position(0, 0) {}

uint32_t RenderableCollectable::get_id() { return id; }

void RenderableCollectable::update(const Gun& snapshot) {
    assert(snapshot.gun_id == id);
    animation.update();
    position.x = snapshot.x;
    position.y = snapshot.y;
}

void RenderableCollectable::render(SDL2pp::Renderer& renderer, Camera& camera) {
    animation.render(renderer, camera, position);
}

std::unordered_map<GunType, std::string> RenderableCollectable::collectable_to_string{
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
        {Armor, "armor"},
        {ActiveGrenade, "active_grenade"},
        {ActiveBanana, "active_banana"},
        {DeathLaser, "death_laser"}};

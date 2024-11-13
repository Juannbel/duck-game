#include "collectable.h"

#include <unordered_map>

#include <yaml-cpp/yaml.h>

#include "SDL2pp/Rect.hh"
#include "client/animation_data_provider.h"
#include "client/textures_provider.h"
#include "common/shared_constants.h"
#include "common/snapshot.h"
#include "../../server/game/gun_types.h"

RenderableCollectable::RenderableCollectable(uint32_t id, GunType type):
        id(id),
        animation(*TexturesProvider::get_texture("collectables"),
                  AnimationDataProvider::get_animation_data("collectables_" +
                                                            collectable_to_string[type])),
        position(0, 0) {
            switch (type) {
                case Grenade:
                    hitbox_width = GRENADE_HITBOX_WIDTH;
                    hitbox_height = GRENADE_HITBOX_HEIGHT;
                    break;
                case Banana:
                    hitbox_width = BANANA_HITBOX_WIDTH;
                    hitbox_height = BANANA_HITBOX_HEIGHT;
                    break;
                case PewPewLaser:
                    hitbox_width = PEWPEW_LASER_HITBOX_WIDTH;
                    hitbox_height = PEWPEW_LASER_HITBOX_HEIGHT;
                    break;
                case LaserRifle:
                    hitbox_width = LASER_RIFLE_HITBOX_WIDTH;
                    hitbox_height = LASER_RIFLE_HITBOX_HEIGHT;
                    break;
                case Ak47:
                    hitbox_width = AK47_HITBOX_WIDTH;
                    hitbox_height = AK47_HITBOX_HEIGHT;
                    break;
                case DuelingPistol:
                    hitbox_width = DUELING_PISTOL_HITBOX_WIDTH;
                    hitbox_height = DUELING_PISTOL_HITBOX_HEIGHT;
                    break;
                case CowboyPistol:
                    hitbox_width = COWBOY_PISTOL_HITBOX_WIDTH;
                    hitbox_height = COWBOY_PISTOL_HITBOX_HEIGHT;
                    break;
                case Magnum:
                    hitbox_width = MAGNUM_HITBOX_WIDTH;
                    hitbox_height = MAGNUM_HITBOX_HEIGHT;
                    break;
                case Shootgun:
                    hitbox_width = SHOTGUN_HITBOX_WIDTH;
                    hitbox_height = SHOTGUN_HITBOX_HEIGHT;
                    break;
                case Sniper:
                    hitbox_width = SNIPER_HITBOX_WIDTH;
                    hitbox_height = SNIPER_HITBOX_HEIGHT;
                    break;
                case Helmet:
                    hitbox_width = HELMET_HITBOX_WIDTH;
                    hitbox_height = HELMET_HITBOX_HEIGHT;
                    break;
                case Armor:
                    hitbox_width = ARMOR_HITBOX_WIDTH;
                    hitbox_height = ARMOR_HITBOX_HEIGHT;
                    break;
                case ActiveGrenade:
                    hitbox_width = GRENADE_HITBOX_WIDTH;
                    hitbox_height = GRENADE_HITBOX_HEIGHT;
                    break;
                case ActiveBanana:
                    hitbox_width = BANANA_HITBOX_WIDTH;
                    hitbox_height = BANANA_HITBOX_HEIGHT;
                    break;
                case DeathLaser:
                    hitbox_width = DEATH_LASER_HITBOX_WIDTH;
                    hitbox_height = DEATH_LASER_HITBOX_HEIGHT;
                    break;
                default:
                    hitbox_width = 0;
                    hitbox_height = 0;
                    break;
            }

        }

uint32_t RenderableCollectable::get_id() { return id; }

void RenderableCollectable::update(const Gun& snapshot) {
    assert(snapshot.gun_id == id);
    animation.update();
    position.x = snapshot.x;
    position.y = snapshot.y;
}

void RenderableCollectable::render(SDL2pp::Renderer& renderer, Camera& camera) {
    // como falta ajustar los offsets, dibujo la hitbox
    // SDL2pp::Rect hitbox(position.x, position.y, COLLECTABLE_HITBOX_WIDTH,
    //                     COLLECTABLE_HITBOX_HEIGHT);
    SDL2pp::Rect hitbox(position.x, position.y, hitbox_width, hitbox_height);
    camera.transform_rect(hitbox);
    renderer.DrawRect(hitbox);

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

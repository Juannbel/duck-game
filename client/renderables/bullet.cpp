#include "bullet.h"

#include <cassert>

#include "client/animation_data_provider.h"
#include "client/textures_provider.h"
#include "common/shared_constants.h"
#include "common/snapshot.h"

RenderableBullet::RenderableBullet(uint32_t id, GunType type):
        id(id),
        animation(*TexturesProvider::get_texture("bullets"),
                  AnimationDataProvider::get_animation_data("bullets_" + gun_to_string[type])),
        position(0, 0),
        angle() {}

uint32_t RenderableBullet::get_id() { return id; }

void RenderableBullet::update(const Bullet& snapshot) {
    assert(snapshot.type != None);
    position.x = snapshot.x;
    position.y = snapshot.y;
    angle = snapshot.angle;

    animation.update();
}

void RenderableBullet::render(SDL2pp::Renderer& renderer, Camera& camera) {
    bool facing_right = angle <= 90 || angle >= 270;
    animation.render(renderer, camera, position, facing_right,
                     facing_right ? (360 - angle) : (180 - angle));


    SDL2pp::Rect hitbox(position.x, position.y, BULLET_HITBOX_WIDTH, BULLET_HITBOX_HEIGHT);
    camera.transform_rect(hitbox);
    renderer.SetDrawColor(255, 0, 0, 255);
    // renderer.FillRect(hitbox);
    renderer.DrawRect(hitbox);
    renderer.SetDrawColor(0, 0, 0, 255);
}

std::unordered_map<GunType, std::string> RenderableBullet::gun_to_string{
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

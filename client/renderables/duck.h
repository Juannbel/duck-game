#ifndef RENDERABLE_DUCK_H
#define RENDERABLE_DUCK_H

#include <string>
#include <unordered_map>

#include <SDL2pp/SDL2pp.hh>
#include <SDL_render.h>
#include <SDL_stdinc.h>

#include "SDL2pp/Point.hh"
#include "client/animation_data_provider.h"
#include "client/renderables/armor.h"
#include "client/renderables/equipped_gun.h"
#include "client/renderables/feathers.h"
#include "client/renderables/helmet.h"
#include "client/renderables/wings.h"
#include "client/textures_provider.h"
#include "common/snapshot.h"

#include "animation.h"

class RenderableDuck {
private:
    uint8_t duck_id;

    Animation* curr_animation;
    RenderableEquippedGun gun;
    RenderableHelmet helmet;
    RenderableArmor armor;
    RenderableWings wings;
    RenderableFeathers feathers;

    std::unordered_map<std::string, Animation*> animations;

    SDL2pp::Point position;

    bool is_facing_right;
    bool is_alive;

    void load_animations(AnimationDataProvider& anim_prov, TexturesProvider& text_prov);

    void load_animation(const std::string& animation_name, AnimationDataProvider& anim_prov, TexturesProvider& text_prov);


public:
    RenderableDuck(uint8_t duck_id, SDL2pp::Renderer& renderer);

    void update(const Duck& duck);

    void render(SDL2pp::Renderer& renderer, Camera& camera);

    void skip_frames(uint8_t frames);

    bool is_dead();

    SDL2pp::Rect get_bounding_box();

    uint8_t get_id();

    ~RenderableDuck();
};

#endif

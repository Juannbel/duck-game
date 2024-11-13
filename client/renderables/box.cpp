#include "box.h"

#include <memory>
#include "client/animation_data_provider.h"
#include "client/textures_provider.h"
#include "common/snapshot.h"

RenderableBox::RenderableBox(uint32_t id) : id(id), position(0, 0) {
    std::shared_ptr<SDL2pp::Texture> texture(TexturesProvider::get_texture("boxes"));
    animations[NoDamage] = new Animation(
        *texture, AnimationDataProvider::get_animation_data("box_no_damage"));
    animations[LittleDamage] = new Animation(
        *texture, AnimationDataProvider::get_animation_data("box_little_damage"));
    animations[MediumDamage] = new Animation(
        *texture, AnimationDataProvider::get_animation_data("box_medium_damage"));
    animations[HeavyDamage] = new Animation(
        *texture, AnimationDataProvider::get_animation_data("box_heavy_damage"));

    curr_animation = animations[NoDamage];
}

void RenderableBox::update(const Box& box_snapshot) {
    position.x = box_snapshot.x;
    position.y = box_snapshot.y;
    curr_animation = animations[box_snapshot.status];
}

void RenderableBox::render(SDL2pp::Renderer& renderer, Camera& camera) {
    curr_animation->render(renderer, camera, position);
}

uint32_t RenderableBox::get_id() {
    return id;
}

RenderableBox::~RenderableBox() {
    for (auto& animation : animations) {
        delete animation.second;
    }
}

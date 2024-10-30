#include "map.h"
#include <string>

#include <SDL2pp/SDL2pp.hh>

#include "client/animation_data_provider.h"
#include "client/renderables/animation.h"
#include "client/textures_provider.h"
#include "common/map_dto.h"
#include "common/shared_constants.h"

#define PARALLAX_FACTOR 0.05f
#define MARGIN 100

RenderableMap::RenderableMap(const Map& map_dto, uint8_t theme) { update(map_dto, theme); }

void RenderableMap::render(SDL2pp::Renderer& renderer, Camera& camera) {
    const SDL2pp::Rect& camera_rect = camera.get_current_rect();
    SDL2pp::Rect background_rect;

    float camera_center_x = camera_rect.x + camera_rect.w / 2.0f;
    float camera_center_y = camera_rect.y + camera_rect.h / 2.0f;

    int texture_width = background_texture->GetWidth();
    int texture_height = background_texture->GetHeight();
    float texture_aspect_ratio = (float)texture_width / texture_height;

    int window_width = renderer.GetOutputWidth();
    int window_height = renderer.GetOutputHeight();
    float window_aspect_ratio = (float)window_width / window_height;

    if (window_aspect_ratio > texture_aspect_ratio) {
        background_rect.w = window_width + 2*MARGIN;
        background_rect.h = static_cast<int>(background_rect.w / texture_aspect_ratio);
    } else {
        background_rect.h = window_height + 2*MARGIN;
        background_rect.w = static_cast<int>(background_rect.h * texture_aspect_ratio);
    }

    background_rect.x = -MARGIN - static_cast<int>(camera_center_x * PARALLAX_FACTOR);
    background_rect.y = -MARGIN - static_cast<int>(camera_center_y * PARALLAX_FACTOR);

    renderer.Copy(*background_texture, SDL2pp::NullOpt, background_rect);

    renderer.SetDrawBlendMode(SDL_BLENDMODE_BLEND);
    renderer.SetDrawColor(0, 0, 0, 80);
    renderer.FillRect(background_rect);
    renderer.SetDrawBlendMode();

    for (auto& block: map) {
        block.render(renderer, camera);
    }
}

std::unordered_map<BlockType, std::string> RenderableMap::block_to_string = {
    {Empty, "empty"},
    {Floor1, "floor_1"},
    {Floor2, "floor_2"},
    {Floor3, "floor_3"},
    {Base1, "base_1"},
    {Base2, "base_2"},
    {Base3, "base_3"},
    {Platform1, "platform_1"},
    {Platform2, "platform_2"},
    {Platform3, "platform_3"},
    {Platform4, "platform_4"},
    {Wall, "wall"},
    {HalfFloor, "half_floor"}
};

void RenderableMap::update(const Map& new_map_dto, uint8_t theme) {
    map.clear();
    background_texture = TexturesProvider::get_texture("background_" + std::to_string(theme));

    std::shared_ptr<SDL2pp::Texture> blocks_texture(
            TexturesProvider::get_texture("blocks"));
    for (int i = 0; i < MAP_HEIGHT_BLOCKS; i++) {
        for (int j = 0; j < MAP_WIDTH_BLOCKS; j++) {
            auto& block = new_map_dto.blocks[i][j];
            if (block.type == Empty) continue;

            AnimationData animation_data(AnimationDataProvider::get_animation_data("blocks_" + std::to_string(theme) + "_" + block_to_string[block.type]));
            SDL2pp::Rect dst_rect(j * BLOCK_SIZE, i * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);

            map.push_back(RenderableBlock(animation_data.frames[0].rect, dst_rect, blocks_texture));
        }
    }
}

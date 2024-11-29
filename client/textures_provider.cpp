#include "textures_provider.h"

#include <cstdint>

#include "common/map_dto.h"
#include "common/config.h"

static Config& config = Config::get_instance();

void TexturesProvider::load_textures(SDL2pp::Renderer& renderer) {
    for (uint8_t i = 0; i < MAP_THEMES; i++) {
        textures["background_" + std::to_string(i)] = std::make_shared<SDL2pp::Texture>(
                renderer, DATA_PATH "/backgrounds/background_" + std::to_string(i) + ".png");
    }
    textures["blocks_solid"] =
            std::make_shared<SDL2pp::Texture>(renderer, DATA_PATH "/sprites/blocks/blocks.png");
    textures["blocks_non_solid"] =
            std::make_shared<SDL2pp::Texture>(renderer, DATA_PATH "/sprites/blocks/blocks.png");
    textures["blocks_non_solid"]->SetAlphaMod(config.get_non_solid_opacity());
    textures["boxes"] =
            std::make_shared<SDL2pp::Texture>(renderer, DATA_PATH "/sprites/boxes/boxes.png");
    textures["duck"] =
            std::make_shared<SDL2pp::Texture>(renderer, DATA_PATH "/sprites/duck/duck_sprite.png");
    textures["guns"] =
            std::make_shared<SDL2pp::Texture>(renderer, DATA_PATH "/sprites/guns/guns.png");
    textures["bullets"] =
            std::make_shared<SDL2pp::Texture>(renderer, DATA_PATH "/sprites/bullets/bullets.png");
    textures["collectables"] = std::make_shared<SDL2pp::Texture>(
            renderer, DATA_PATH "/sprites/collectables/collectables.png");
    textures["wings"] =
            std::make_shared<SDL2pp::Texture>(renderer, DATA_PATH "/sprites/wings/wings.png");
    textures["feathers"] =
            std::make_shared<SDL2pp::Texture>(renderer, DATA_PATH "/sprites/feathers/feathers.png");
    textures["helmet"] =
            std::make_shared<SDL2pp::Texture>(renderer, DATA_PATH "/sprites/equipment/helmet.png");
    textures["armor"] =
            std::make_shared<SDL2pp::Texture>(renderer, DATA_PATH "/sprites/equipment/armor.png");
}

std::shared_ptr<SDL2pp::Texture> TexturesProvider::get_texture(const std::string& texture_name) {
    return textures[texture_name];
}

TexturesProvider::~TexturesProvider() {
    // No hace falta ir haciendo los delete por usar smart pointers
    textures.clear();
}

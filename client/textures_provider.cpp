#include "textures_provider.h"

std::unordered_map<std::string, SDL2pp::Texture*> TexturesProvider::textures;

void TexturesProvider::loadTextures(SDL2pp::Renderer& renderer) {
    textures["background_0"] = new SDL2pp::Texture(renderer, DATA_PATH "/backgrounds/background_0.png");
    textures["blocks_0"] = new SDL2pp::Texture(renderer, DATA_PATH "/sprites/blocks/blocks_0.png");
    textures["duck"] = new SDL2pp::Texture(renderer, DATA_PATH "/sprites/duck/duck_sprite.png");
    textures["guns"] = new SDL2pp::Texture(renderer, DATA_PATH "/sprites/guns/guns.png");
    textures["bullets"] = new SDL2pp::Texture(renderer, DATA_PATH "/sprites/bullets/bullets.png");
    textures["collectables"] = new SDL2pp::Texture(renderer, DATA_PATH "/sprites/collectables/collectables.png");
    textures["wings"] = new SDL2pp::Texture(renderer, DATA_PATH "/sprites/wings/wings.png");
    textures["helmet"] = new SDL2pp::Texture(renderer, DATA_PATH "/sprites/equipment/helmet.png");
}

SDL2pp::Texture* TexturesProvider::getTexture(const std::string& texture_name) {
    return textures[texture_name];
}

TexturesProvider::~TexturesProvider() {
    for (auto& texture_pair : textures) {
        delete texture_pair.second;
    }
    textures.clear();
}

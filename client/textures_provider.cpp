#include "textures_provider.h"

std::unordered_map<std::string, SDL2pp::Texture*> TexturesProvider::textures;

void TexturesProvider::loadTextures(SDL2pp::Renderer& renderer) {
    textures["duck"] = new SDL2pp::Texture(renderer, DATA_PATH "/sprites/duck/duck_sprite.png");
    textures["forest_background"] = new SDL2pp::Texture(renderer, DATA_PATH "/backgrounds/forest.png");
    textures["blocks"] = new SDL2pp::Texture(renderer, DATA_PATH "/sprites/tiles/tiles.png");
    textures["guns"] = new SDL2pp::Texture(renderer, DATA_PATH "/sprites/guns/guns.png");
    textures["collectables"] = new SDL2pp::Texture(renderer, DATA_PATH "/sprites/collectables/collectables.png");
}

SDL2pp::Texture* TexturesProvider::getTexture(const std::string& texture_name) {
    auto it = textures.find(texture_name);
    if (it != textures.end()) {
        return it->second;
    }
    return nullptr;  // O maneja el caso de error si no se encuentra la textura
}

TexturesProvider::~TexturesProvider() {
    for (auto& texture_pair : textures) {
        delete texture_pair.second;
    }
}

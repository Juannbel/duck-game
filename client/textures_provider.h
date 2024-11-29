#ifndef TEXTURES_PROVIDER_H
#define TEXTURES_PROVIDER_H

#include <memory>
#include <string>
#include <unordered_map>

#include "SDL2pp/Renderer.hh"
#include "SDL2pp/Texture.hh"

class TexturesProvider {
private:
    // El map para almacenar las texturas, clave: nombre de la textura, valor: puntero a la textura
    std::unordered_map<std::string, std::shared_ptr<SDL2pp::Texture>> textures;

    explicit TexturesProvider(SDL2pp::Renderer& renderer) { load_textures(renderer); }

public:
    static TexturesProvider& get_instance(SDL2pp::Renderer& renderer) {
        static TexturesProvider instance(renderer);
        return instance;
    }

    // Método para obtener una textura por nombre
    std::shared_ptr<SDL2pp::Texture> get_texture(const std::string& texture_name);

    // Método para cargar todas las texturas al iniciar la aplicación
    void load_textures(SDL2pp::Renderer& renderer);

    ~TexturesProvider();
};

#endif

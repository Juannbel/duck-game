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
    static std::unordered_map<std::string, std::shared_ptr<SDL2pp::Texture>> textures;

    // Constructor privado para evitar instancias
    TexturesProvider() {}

public:
    // Método para obtener una textura por nombre
    static std::shared_ptr<SDL2pp::Texture> get_texture(const std::string& texture_name);

    // Método para cargar todas las texturas al iniciar la aplicación
    static void load_textures(SDL2pp::Renderer& renderer);

    // Destructor para liberar la memoria al cerrar la aplicación
    ~TexturesProvider();
};

#endif

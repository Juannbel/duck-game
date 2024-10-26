#ifndef TEXTURES_PROVIDER_H
#define TEXTURES_PROVIDER_H

#include <string>
#include <unordered_map>
#include "SDL2pp/Renderer.hh"
#include "SDL2pp/Texture.hh"

class TexturesProvider {
private:
    // El map para almacenar las texturas, clave: nombre de la textura, valor: puntero a la textura
    static std::unordered_map<std::string, SDL2pp::Texture*> textures;

    // Constructor privado para evitar instancias
    TexturesProvider() {}

public:
    // Método para obtener una textura por nombre
    static SDL2pp::Texture* getTexture(const std::string& texture_name);

    // Método para cargar todas las texturas al iniciar la aplicación
    static void loadTextures(SDL2pp::Renderer& renderer);

    // Destructor para liberar la memoria al cerrar la aplicación
    ~TexturesProvider();
};

#endif

#ifndef JOYSTICK_MANAGER_H
#define JOYSTICK_MANAGER_H

#include <SDL_events.h>
#include <SDL_gamecontroller.h>
#include <SDL_joystick.h>

struct JoystickInstance {
    SDL_GameController* joystick;
    SDL_JoystickID instance_id;
};

class JoystickManager {
private:
    JoystickInstance* primary_instance;
    JoystickInstance* secondary_instance;

    // Trata de asignar a la instancia el joystick de id "id"
    // Devuelve true si efectivamente se asigno
    bool try_assign_joystick(JoystickInstance* inst, int id);

    // Maneja el evento generado al desconectar un controller
    // Si el controller era usado por una instancia, se los desconecta de la misma
    void handle_removed(const SDL_Event& event);

    // Maneja el evento generado al conectar un controller
    // Si hay una instancia sin joystick, tratará de asignarlo a ella
    void handle_added(const SDL_Event& event);

    // cierra el joystick representado en instance
    // deja la instancia como sin joystick
    void close(JoystickInstance* instance);

public:
    // Primero viene el joystick del p1, despues el del p2
    // el de p2 puede ser NULL si no esta conectado
    // Verifica la configuración para el caso
    // que hay dos jugadores y un joystick
    JoystickManager(JoystickInstance* p1, JoystickInstance* p2);

    void handle_event(const SDL_Event& event);

    ~JoystickManager();
};

#endif

#include "joystick_manager.h"

#include "common/config.h"

const static uint8_t FIRST_GAMEPAD_PLAYER = Config::get_instance().get_first_gamepad_player();

JoystickManager::JoystickManager(JoystickInstance* p1, JoystickInstance* p2) {
    primary_instance = FIRST_GAMEPAD_PLAYER == 0 ? p1 : p2;
    secondary_instance = FIRST_GAMEPAD_PLAYER == 0 ? p2 : p1;
}

bool JoystickManager::try_assign_joystick(JoystickInstance* inst, int id) {
    if (inst->joystick)
        return false;

    if ((inst->joystick = SDL_GameControllerOpen(id)) != NULL)
        inst->instance_id = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(inst->joystick));

    // o ya esta asignado o no lo pudimos abrir
    return true;
}

void JoystickManager::handle_event(const SDL_Event& event) {
    switch (event.type) {
        case SDL_CONTROLLERDEVICEREMOVED: {
            handle_removed(event);
            break;
        }

        case SDL_CONTROLLERDEVICEADDED: {
            // Si se lanza este event, es porque es GameController
            handle_added(event);
            break;
        }
    }
}

void JoystickManager::handle_removed(const SDL_Event& event) {
    const auto& removed_id = event.cdevice.which;
    auto used = primary_instance && primary_instance->instance_id == removed_id ? primary_instance :
                                                                                  NULL;

    if (!used && secondary_instance)
        used = secondary_instance->instance_id == removed_id ? secondary_instance : NULL;

    if (used) {
        close(used);
    }
}

void JoystickManager::handle_added(const SDL_Event& event) {
    int id = event.cdevice.which;

    if (primary_instance && try_assign_joystick(primary_instance, id))
        return;

    if (secondary_instance)
        try_assign_joystick(secondary_instance, id);
}

void JoystickManager::close(JoystickInstance* instance) {
    SDL_GameControllerClose(instance->joystick);
    instance->joystick = nullptr;
    instance->instance_id = -1;
}

JoystickManager::~JoystickManager() {
    if (primary_instance && primary_instance->joystick) {
        close(primary_instance);
    }

    if (secondary_instance && secondary_instance->joystick) {
        close(secondary_instance);
    }
}

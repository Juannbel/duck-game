#include <cstdlib>
#include <iostream>

#include <SDL2/SDL_gamecontroller.h>
#include <qapplication.h>

#include "pre_lobby/pre_lobby.h"

#include "client.h"

#define ARG_HOSTNAME 1
#define ARG_SERVNAME 2

int main(int argc, char* argv[]) {
    if (argc != 1) {
        std::cerr << "Bad call. Usage: " << argv[0] << ". No args expected" << std::endl;
        return EXIT_FAILURE;
    }

    try {
        QApplication app(argc, argv);
        bool connected = false;
        PreLobby pre_lobby(app, connected);
        pre_lobby.run();

        if (!connected) {
            return EXIT_SUCCESS;
        }

        SDL_GameControllerAddMappingsFromFile(DATA_PATH "/gamepad_mappings.txt");

        bool play_again = true;
        while (play_again) {
            Client client(app, pre_lobby.get_socket());
            play_again = client.run();
        }

        return EXIT_SUCCESS;

    } catch (const std::exception& e) {
        std::cerr << "Something went wrong: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Something went wrong" << std::endl;
        return EXIT_FAILURE;
    }
}

#include "client.h"

#include <cstdint>
#include <vector>
#include "common/lobby.h"
#include "lobby/lobby.h"

#include "constant_looper.h"

Client::Client(const char* hostname, const char* servname):
        protocol(Socket(hostname, servname)),
        receiver(protocol, snapshot_q),
        sender(protocol, command_q) {}

void Client::run() {
    int argc = 0;  // Asegúrate de que estas variables se inicialicen correctamente
    char** argv = nullptr;

    uint8_t duck_id;
    Lobby lobby(argc, argv, protocol, duck_id);
    lobby.run();

    // while (true) {
    //     int32_t option = display_menu_and_get_option();
    //     protocol.send_option(option);

    //     if (option == CREATE_GAME) {
    //         GameInfo game_info = protocol.recv_game_info();
    //         duck_id = game_info.duck_id;

    //         std::cout << "Game created with id: " << game_info.game_id << std::endl;
    //         std::cout << "Press any key to start..." << std::endl;

    //         std::cin.ignore();
    //         std::cin.get();

    //         protocol.send_option(0);
    //         break;
    //     } else if (option == LIST_GAMES) {
    //         display_lobbies();
    //         continue;
    //     } else if (option == JOIN_GAME) {
    //         if (!joinLobby(duck_id)) {
    //             continue;
    //         }
    //         break;
    //     }
    // }

    receiver.start();
    sender.start();

    ConstantLooper looper(duck_id, snapshot_q, command_q);
    looper.run();

    std::cout << "Game ended" << std::endl;
}

int Client::display_menu_and_get_option() {
    std::cout << "1 - List games" << std::endl;
    std::cout << "2 - Create Game" << std::endl;
    std::cout << "3 - Join Game" << std::endl;

    int32_t option;
    do {
        std::cout << "Enter an option (1, 2 or 3): ";
        std::cin >> option;
    } while (option != 1 && option != 2 && option != 3);

    return option;
}

bool Client::joinLobby(uint8_t &duck_id) {
    int32_t lobbyId = get_lobby_id();
    protocol.send_option(lobbyId);
    GameInfo game_info = protocol.recv_game_info();
    if (game_info.game_id == INVALID_GAME_ID) {
        std::cout << "Failed to join" << std::endl;
        return false;
    }

    duck_id = game_info.duck_id;
    return true;
}

void Client::display_lobbies() {
    std::vector<int32_t> lobbies = protocol.recv_lobbies_info();

    if (lobbies.size() == 0) {
        std::cout << "No lobbies available" << std::endl;
        return;
    }

    for (int32_t lobby : lobbies) {
        std::cout << "Lobby: " << lobby << std::endl;
    }
}

int32_t Client::get_lobby_id() {
    int32_t lobbyId;
    std::cout << "Enter an option (id of lobby): ";
    std::cin >> lobbyId;
    return lobbyId;
}

Client::~Client() {
    protocol.shutdown();
    snapshot_q.close();
    command_q.close();
    receiver.stop();
    sender.stop();
    receiver.join();
    sender.join();
}

#include "client.h"
#include "common/map_dto.h"

#include "constant_looper.h"

#define JOIN 2

Client::Client(const char* hostname, const char* servname):
        protocol(Socket(hostname, servname)),
        receiver(protocol, snapshot_q),
        sender(protocol, command_q) {}

void Client::run() {
    const int option = displayMenuAndGetOption();
    protocol.send_option(option);
    
    if (option == JOIN) {
        joinLobby();
    }

    MatchInfo match_info = protocol.recv_match_info();
    std::cout << "Match info received" << std::endl;
    std::cout << "Duck id: " << static_cast<int>(match_info.duck_id) << std::endl;

    receiver.start();
    sender.start();

    ConstantLooper looper(match_info, snapshot_q, command_q);
    looper.run();

    std::cout << "Game ended" << std::endl;
}

int Client::displayMenuAndGetOption() {
    std::cout << "1 - Create Lobby" << std::endl;
    std::cout << "2 - Join a Lobby" << std::endl;
    int option;
    std::cout << "Enter an option (1 or 2): ";
    std::cin >> option;
    return option;
}

void Client::joinLobby() {
    displayLobbyList();
    const int lobbyId = getLobbyIdFromUser();
    protocol.send_option(lobbyId);
}

void Client::displayLobbyList() {
    int id = protocol.recv_lobby();
    while (id != -1) {
        std::cout << "Lobby: " << id << std::endl;
        id = protocol.recv_lobby();
    }
}

int Client::getLobbyIdFromUser() {
    int lobbyId;
    std::cout << "Enter an option (id of lobby): ";
    std::cin >> lobbyId;
    return lobbyId;
}

Client::~Client() {
    protocol.shutdown();
    snapshot_q.close();
    command_q.close();
    receiver.join();
    sender.join();
}

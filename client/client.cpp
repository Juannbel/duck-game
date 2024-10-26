#include "client.h"
#include "common/map_dto.h"

#include "constant_looper.h"

Client::Client(const char* hostname, const char* servname):
        protocol(Socket(hostname, servname)),
        receiver(protocol, snapshot_q),
        sender(protocol, command_q) {}

void Client::run() {
    // TODO: Esto esta horrible cambiarlo (pero funciona)
    std::cout << "1 - Create Lobby" << std::endl;
    std::cout << "2 - Join a Lobby" << std::endl;
    int option;
    std::cout << "Enter an option (1 or 2): " << std::endl;
    std::cin >> option;
    protocol.send_option(option);
    // Si es unirse, recibir lista de lobbies
    if (option == 2) {
        int id = protocol.recv_lobby();
        while (id != -1) {
            // mostrar lista de lobbies
            std::cout << "Lobby: " << id << std::endl;
            id = protocol.recv_lobby();
        }
        std::cout << "Enter an option (id of lobby): " << std::endl;
        std::cin >> option;
        // enviar id del lobby al que se quiere unir
        protocol.send_option(option);
    }
    
    MatchInfo match_info = protocol.recv_match_info();
    std::cout << "Match info received" << std::endl;
    std::cout << "Duck id: " << (int) match_info.duck_id << std::endl;

    receiver.start();
    sender.start();

    ConstantLooper looper(match_info, snapshot_q, command_q);
    looper.run();

    std::cout << "Game ended" << std::endl;
}

Client::~Client() {
    protocol.shutdown();
    snapshot_q.close();
    command_q.close();
    receiver.join();
    sender.join();
}

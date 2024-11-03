#include "client.h"

#include <cstdint>
#include "common/lobby.h"
#include "lobby/lobby.h"

#include "constant_looper.h"

Client::Client(const char* hostname, const char* servname):
        protocol(Socket(hostname, servname)),
        receiver(protocol, snapshot_q),
        sender(protocol, command_q) {}

void Client::run() {
    uint8_t duck_id = INVALID_DUCK_ID;
    Lobby lobby(protocol, duck_id);
    lobby.run();

    receiver.start();
    sender.start();

    ConstantLooper looper(duck_id, snapshot_q, command_q);
    looper.run();

    std::cout << "Game ended" << std::endl;
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

#include "client.h"

#include <cstdint>
#include "common/blocking_queue.h"
#include "common/lobby.h"
#include "lobby/lobby.h"

#include "constant_looper.h"

Client::Client(const char* hostname, const char* servname):
        alive(true),
        protocol(Socket(hostname, servname)),
        receiver(protocol, snapshot_q, alive),
        sender(protocol, command_q, alive) {}

void Client::run() {
    uint8_t duck_id = INVALID_DUCK_ID;
    Lobby lobby(protocol, duck_id);
    lobby.run();

    receiver.start();
    sender.start();

    if (duck_id == INVALID_DUCK_ID) return;

    ConstantLooper looper(duck_id, snapshot_q, command_q);
    looper.run();

    std::cout << "Game ended" << std::endl;
}

Client::~Client() {
    protocol.shutdown();
    try { snapshot_q.close(); } catch (...) {};
    try { command_q.close(); } catch (...) {};
    receiver.stop();
    sender.stop();
    receiver.join();
    sender.join();
}

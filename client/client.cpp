#include "client.h"

#include <array>
#include <cstdint>

#include "common/blocking_queue.h"
#include "common/lobby.h"
#include "lobby/lobby.h"

#include "constant_looper.h"

Client::Client(const char* hostname, const char* servname):
        alive(true),
        protocol(Socket(hostname, servname)),
        receiver(protocol, snapshot_q, alive),
        sender(protocol, actions_q, alive) {}

void Client::run() {
    std::pair<uint8_t, uint8_t> duck_ids = {INVALID_DUCK_ID, INVALID_DUCK_ID};
    Lobby lobby(protocol, duck_ids);
    lobby.run();

    receiver.start();
    sender.start();

    if (duck_ids.first == INVALID_DUCK_ID)
        return;

    ConstantLooper looper(duck_ids, snapshot_q, actions_q);
    looper.run();

    std::cout << "Game ended" << std::endl;
}

Client::~Client() {
    protocol.shutdown();
    try {
        snapshot_q.close();
    } catch (...) {};
    try {
        actions_q.close();
    } catch (...) {};
    receiver.stop();
    sender.stop();
    receiver.join();
    sender.join();
}

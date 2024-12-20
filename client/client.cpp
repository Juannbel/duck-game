#include "client.h"

#include <cstdint>
#include <utility>

#include "common/blocking_queue.h"
#include "common/lobby.h"
#include "lobby/lobby.h"

#include "constant_looper.h"

Client::Client(QApplication& app, Socket&& socket):
        alive(true),
        protocol(std::move(socket)),
        receiver(protocol, snapshot_q, alive),
        sender(protocol, actions_q, alive),
        app(app) {}

bool Client::run() {
    std::pair<uint8_t, uint8_t> duck_ids = {INVALID_DUCK_ID, INVALID_DUCK_ID};
    bool ready_to_play = false;
    Lobby lobby(app, protocol, duck_ids, ready_to_play);
    lobby.run();

    receiver.start();
    sender.start();

    if (!ready_to_play)
        return false;

    ConstantLooper looper(duck_ids, snapshot_q, actions_q);
    return looper.run();
}

Client::~Client() {
    protocol.shutdown();
    try {
        snapshot_q.close();
    } catch (...) {}
    try {
        actions_q.close();
    } catch (...) {}
    receiver.stop();
    sender.stop();
    receiver.join();
    sender.join();
}

#ifndef RECEIVER_H
#define RECEIVER_H

// TODO: Todos los includes necesarios
#include "common/blocking_queue.h"
#include "common/thread.h"

#include "server_protocol.h"

class ServerReceiver: public Thread {
private:
    ServerProtocol& protocol;
    Queue<struct action>& gameloop_q;
    uint8_t duck_id;

public:
    ServerReceiver(ServerProtocol& protocol, Queue<struct action>& q, uint8_t duck_id);

    void run() override;
};

#endif

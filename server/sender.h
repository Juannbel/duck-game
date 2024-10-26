#ifndef SENDER_H
#define SENDER_H

// TODO: Todos los includes necesarios
#include "common/blocking_queue.h"
#include "common/snapshot.h"
#include "common/thread.h"

#include "server_protocol.h"

class ServerSender: public Thread {
private:
    ServerProtocol& protocol;
    Queue<Snapshot>& sender_q;
    uint8_t duck_id;

public:
    ServerSender(ServerProtocol& protocol, Queue<Snapshot>& sender_q);

    void set_duck_id(uint8_t duck_id) { this->duck_id = duck_id; }

    void run() override;
};

#endif

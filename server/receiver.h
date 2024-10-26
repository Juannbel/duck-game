#ifndef RECEIVER_H
#define RECEIVER_H

#include "common/blocking_queue.h"
#include "common/thread.h"

#include "server_protocol.h"

class ServerReceiver: public Thread {
private:
    ServerProtocol& protocol;
    Queue<struct action>* gameloop_q;
    uint8_t duck_id;

public:
    ServerReceiver(ServerProtocol& protocol);

    void set_duck_id(uint8_t duck_id) { this->duck_id = duck_id; }

    void set_gameloop_queue(Queue<struct action>* queue);

    void run() override;
};

#endif

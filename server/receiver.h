#ifndef RECEIVER_H
#define RECEIVER_H

// TODO: Todos los includes necesarios
#include "common/blocking_queue.h"
#include "common/snapshot.h"
#include "common/thread.h"

#include "serverProtocol.h"

class Receiver: public Thread {
private:
    ServerProtocol& protocol;
    Queue<struct action>& gameloop_q;

public:
    Receiver(ServerProtocol& protocol, Queue<struct action>& q);

    void run() override;
};

#endif

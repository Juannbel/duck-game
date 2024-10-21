#ifndef SENDER_H
#define SENDER_H

// TODO: Todos los includes necesarios
#include "common/blocking_queue.h"
#include "common/thread.h"

#include "common/snapshot.h"
#include "serverProtocol.h"

class Sender: public Thread {
private:
    ServerProtocol& protocolo;
    Queue<Snapshot>& sender_q;

public:
    Sender(ServerProtocol& protocolo, Queue<Snapshot>& sender_q);

    void run() override;
};

#endif

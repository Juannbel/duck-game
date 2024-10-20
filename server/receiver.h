#ifndef RECEIVER_H
#define RECEIVER_H

// TODO: Todos los includes necesarios
#include "common/blocking_queue.h"
#include "common/snapshot.h"
#include "common/thread.h"

#include "common/snapshot.h"
#include "serverProtocolo.h"

class Receiver: public Thread {
private:
    ServerProtocolo& protocolo;
    Queue<Snapshot>& gameloop_q;

public:
    Receiver(ServerProtocolo& protocolo, Queue<Snapshot>& q);

    void run() override;
};

#endif

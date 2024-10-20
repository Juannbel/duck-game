#ifndef SENDER_H
#define SENDER_H

// TODO: Todos los includes necesarios
#include "libs/queue.h"
#include "libs/thread.h"

#include "common/snapshot.h"
#include "serverProtocolo.h"

class Sender: public Thread {
private:
    ServerProtocolo& protocolo;
    Queue<Snapshot>& sender_q;

public:
    Sender(ServerProtocolo& protocolo, Queue<Snapshot>& sender_q);

    void run() override;
};

#endif

#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H

// TODO: Todos los includes necesarios
#include "common/blocking_queue.h"
#include "common/snapshot.h"
#include "common/thread.h"

#include "clientProtocol.h"

class ClientReceiver: public Thread {
private:
    ClientProtocol& protocol;
    Queue<Snapshot>& snapshot_q;

public:
    ClientReceiver(ClientProtocol& protocol, Queue<Snapshot>& q);

    void run() override;
};

#endif

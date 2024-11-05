#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H

// TODO: Todos los includes necesarios
#include <atomic>
#include "common/blocking_queue.h"
#include "common/snapshot.h"
#include "common/thread.h"

#include "client_protocol.h"

class ClientReceiver: public Thread {
private:
    ClientProtocol& protocol;
    Queue<Snapshot>& snapshot_q;
    std::atomic<bool>& alive;

public:
    ClientReceiver(ClientProtocol& protocol, Queue<Snapshot>& q, std::atomic<bool>& alive);

    void run() override;
};

#endif

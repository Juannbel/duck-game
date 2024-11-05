#ifndef CLIENT_SENDER_H
#define CLIENT_SENDER_H

#include "common/blocking_queue.h"
#include "common/thread.h"

#include "client_protocol.h"

class ClientSender: public Thread {
private:
    ClientProtocol& protocol;
    Queue<Command>& sender_q;
    std::atomic<bool>& alive;

public:
    ClientSender(ClientProtocol& protocolo, Queue<Command>& sender_q, std::atomic<bool>& alive);

    void run() override;
};

#endif

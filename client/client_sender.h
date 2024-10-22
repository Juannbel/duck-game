#ifndef CLIENT_SENDER_H
#define CLIENT_SENDER_H

#include "../common/blocking_queue.h"
#include "../common/snapshot.h"
#include "../common/thread.h"

#include "clientProtocol.h"

class ClientSender: public Thread {
private:
    ClientProtocol& protocol;
    Queue<Command>& sender_q;

public:
    ClientSender(ClientProtocol& protocolo, Queue<Command>& sender_q);

    void run() override;
};

#endif

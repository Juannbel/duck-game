#ifndef CLIENT_H
#define CLIENT_H

#include <atomic>

#include "client_protocol.h"
#include "client_receiver.h"
#include "client_sender.h"

class Client {
private:
    std::atomic<bool> alive;
    ClientProtocol protocol;
    Queue<action> actions_q;
    Queue<Snapshot> snapshot_q;
    ClientReceiver receiver;
    ClientSender sender;

public:
    Client(const char* hostname, const char* servname);

    void run();

    ~Client();
};

#endif

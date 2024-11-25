#ifndef CLIENT_H
#define CLIENT_H

#include <atomic>
#include <qapplication.h>

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
    QApplication& app;

public:
    Client(QApplication& app, Socket&& socket);

    bool run();

    ~Client();
};

#endif

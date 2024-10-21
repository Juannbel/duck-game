#ifndef SERVER_H
#define SERVER_H

// TODO: Todos los includes necesarios
#include "acceptor.h"
#include "gameloop.h"
#include "socket/socket.h"

class Server {
private:
    Socket sk;
    QueueListMonitor sv_msg_queues;
    Gameloop gameloop;
    Acceptor thAcceptor;

public:
    void start();

    explicit Server(const char* puerto);

    ~Server();
};

#endif

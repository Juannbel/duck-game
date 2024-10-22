#ifndef SERVER_H
#define SERVER_H

// TODO: Todos los includes necesarios
#include "common/socket.h"

#include "acceptor.h"
#include "game_loop.h"

class Server {
private:
    Socket sk;
    QueueListMonitor sv_msg_queues;
    Queue<struct action> actions_q;
    GameLoop gameloop;
    Acceptor thAcceptor;

public:
    void start();

    explicit Server(const char* port);

    ~Server();
};

#endif

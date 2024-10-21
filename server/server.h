#ifndef SERVER_H
#define SERVER_H

// TODO: Todos los includes necesarios
#include "acceptor.h"
#include "game_loop.h"
#include "common/socket.h"

class Server {
private:
    Socket sk;
    QueueListMonitor sv_msg_queues;
    Queue <struct action> actions_q;
    GameLoop gameloop;
    Acceptor thAcceptor;

public:
    void start();

    explicit Server(const char* puerto);

    ~Server();
};

#endif

#ifndef SERVER_H
#define SERVER_H

// TODO: Todos los includes necesarios
#include "aceptador.h"
#include "gameloop.h"
#include "socket/socket.h"

class Server {
private:
    Socket sk;
    QueueListMonitor sv_msg_queues;
    Gameloop gameloop;
    Aceptador thAceptador;

public:
    void start();

    explicit Server(const char* puerto);

    ~Server();
};

#endif

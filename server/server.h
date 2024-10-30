#ifndef SERVER_H
#define SERVER_H

// TODO: Todos los includes necesarios
#include "common/socket.h"
#include "acceptor.h"

class Server {
private:
    Socket sk;
    Acceptor thAcceptor;
public:
    void start();

    explicit Server(const char* port);

    ~Server();
};

#endif

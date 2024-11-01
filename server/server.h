#ifndef SERVER_H
#define SERVER_H

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

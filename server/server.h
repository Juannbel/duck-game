#ifndef SERVER_H
#define SERVER_H

// TODO: Todos los includes necesarios
#include "common/socket.h"
#include "server/maps/yaml.h"

#include "acceptor.h"

class Server {
private:
    Socket sk;
    QueueListMonitor sv_msg_queues;
    Queue<struct action> actions_q;
    YAMLLoader map_loader;
    Map map;
    Acceptor thAcceptor;


public:
    void start();

    explicit Server(const char* port);

    ~Server();
};

#endif

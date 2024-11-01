#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <list>

#include "common/socket.h"
#include "common/thread.h"

#include "server_client.h"

class Acceptor: public Thread {
private:
    Socket& sk;
    std::list<ServerClient*> players;

public:
    explicit Acceptor(Socket& sk);
    void run() override;
    void reap_dead();
    void kill_all();
    int get_clients_count();
};

#endif

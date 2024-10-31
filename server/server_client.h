#ifndef SERVER_CLIENT_H
#define SERVER_CLIENT_H

#include "receiver.h"
#include "common/blocking_queue.h"
#include "common/snapshot.h"
#include "common/socket.h"
#include "server_protocol.h"
#include "games_monitor.h"

class ServerClient {
private:
    Socket sk;  // peer skt

    Queue<Snapshot> sender_q;
    ServerProtocol protocolo;
    ServerReceiver receiver;
    std::atomic<bool> is_alive;

public:
    ServerClient(Socket&& sk, GamesMonitor& games_monitor, int id);
    void start();
    void join();
    bool is_dead();
    void kill();
};

#endif

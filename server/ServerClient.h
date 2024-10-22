#ifndef SERVER_CLIENT_H
#define SERVER_CLIENT_H

// TODO: Todos los includes necesarios
#include "common/blocking_queue.h"
#include "common/snapshot.h"
#include "common/socket.h"
#include "common/thread.h"

#include "receiver.h"
#include "sender.h"
#include "serverProtocol.h"

class ServerClient {
private:
    Socket sk;  // peer skt

    Queue<Snapshot> sender_q;
    ServerProtocol protocolo;
    Sender sender;
    Receiver receiver;

    int id;

    std::atomic<bool> is_alive;

public:
    ServerClient(Socket&& sk, Queue<struct action>& gameloop_q, int id);
    Queue<Snapshot>& get_sender_queue() { return sender_q; }
    int get_id() { return id; }
    void start();
    void join();
    bool is_dead();
    void kill();
};

#endif

#ifndef SERVER_CLIENT_H
#define SERVER_CLIENT_H

// TODO: Todos los includes necesarios
#include "libs/queue.h"
#include "libs/thread.h"

#include "common/snapshot.h"
#include "serverProtocolo.h"
#include "receiver.h"
#include "sender.h"
#include "socket/socket.h"

class ServerClient {
private:
    Socket sk;  // peer skt

    Queue<Snapshot> sender_q;
    ServerProtocolo protocolo;
    Sender sender;
    Receiver receiver;

    int id;

    std::atomic<bool> is_alive;

public:
    ServerClient(Socket sk, Queue<Snapshot>& gameloop_q, int id);
    Queue<Snapshot>& get_sender_queue() { return sender_q; }
    int get_id() { return id; }
    void start();
    void join();
    bool is_dead();
    void kill();
};

#endif

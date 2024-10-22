#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <list>

// TODO: Todos los includes necesarios
#include "common/blocking_queue.h"
#include "common/snapshot.h"
#include "common/socket.h"
#include "common/thread.h"

#include "ServerClient.h"
#include "list_monitor.h"

class Acceptor: public Thread {
private:
    Socket& sk;
    std::list<ServerClient*> clients;
    Queue<struct action>& gameloop_q;

    QueueListMonitor& sv_msg_queues;

public:
    Acceptor(Socket& sk, Queue<struct action>& gameloop_q, QueueListMonitor& sv_msg_queues);
    void run() override;
    void reap_dead();
    void kill_all();
};

#endif
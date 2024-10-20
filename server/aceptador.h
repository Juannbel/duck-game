#ifndef ACEPTADOR_H
#define ACEPTADOR_H

#include <list>

// TODO: Todos los includes necesarios
#include "list_monitor.h"
#include "common/thread.h"
#include "common/blocking_queue.h"
#include "common/snapshot.h"

#include "ServerClient.h"
#include "socket/socket.h"

class Aceptador: public Thread {
private:
    Socket& sk;
    std::list<ServerClient*> clients;  // cppcheck-suppress unusedStructMember
    Queue<Snapshot>& gameloop_q;

    QueueListMonitor& sv_msg_queues;

public:
    Aceptador(Socket& sk, Queue<Snapshot>& gameloop_q, QueueListMonitor& sv_msg_queues);
    void run() override;
    void reap_dead();
    void kill_all();
};

#endif

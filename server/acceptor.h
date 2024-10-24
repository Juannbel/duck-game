#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <list>

// TODO: Todos los includes necesarios
#include "common/blocking_queue.h"
#include "common/map_dto.h"
#include "common/socket.h"
#include "common/thread.h"

#include "server_client.h"
#include "list_monitor.h"

class Acceptor: public Thread {
private:
    Socket& sk;
    std::list<ServerClient*> clients;
    Queue<struct action>& gameloop_q;

    QueueListMonitor& sv_msg_queues;
    Map& map;

public:
    Acceptor(Socket& sk, Queue<struct action>& gameloop_q, QueueListMonitor& sv_msg_queues, Map& map);
    void run() override;
    void reap_dead();
    void kill_all();
    int get_clients_count();
};

#endif

#ifndef SERVER_CLIENT_H
#define SERVER_CLIENT_H

// TODO: Todos los includes necesarios
#include "common/blocking_queue.h"
#include "common/map_dto.h"
#include "common/snapshot.h"
#include "common/socket.h"

#include "receiver.h"
#include "sender.h"
#include "server_protocol.h"

class ServerClient {
private:
    Socket sk;  // peer skt

    Queue<Snapshot> sender_q;
    ServerProtocol protocolo;
    ServerSender sender;
    ServerReceiver receiver;

    int id;

    std::atomic<bool> is_alive;

public:
    ServerClient(Socket&& sk, Queue<struct action>& gameloop_q, int id, MatchInfo match_info);
    Queue<Snapshot>& get_sender_queue() { return sender_q; }
    int get_id() { return id; }
    void start();
    void join();
    bool is_dead();
    void kill();
};

#endif

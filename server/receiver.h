#ifndef SERVER_RECEIVER_H
#define SERVER_RECEIVER_H

#include "common/blocking_queue.h"
#include "common/thread.h"

#include "server_protocol.h"
#include "games_monitor.h"
#include "sender.h"


class ServerReceiver: public Thread {
private:
    ServerProtocol& protocol;
    Queue<action>* gameloop_q;
    GamesMonitor& games_monitor;
    uint8_t duck_id;
    Queue<Snapshot>& sender_q;
    ServerSender sender;

public:
    ServerReceiver(ServerProtocol& protocol, GamesMonitor& games_monitor, Queue<Snapshot>& sender_q, uint8_t playerId);

    void setup_game();

    void run() override;

    ~ServerReceiver();
};

#endif

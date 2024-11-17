#ifndef SERVER_RECEIVER_H
#define SERVER_RECEIVER_H

#include <cstdint>

#include <sys/types.h>

#include "common/blocking_queue.h"
#include "common/thread.h"

#include "games_monitor.h"
#include "sender.h"
#include "server_protocol.h"


class ServerReceiver: public Thread {
private:
    ServerProtocol& protocol;
    Queue<action>* gameloop_q;
    GamesMonitor& games_monitor;
    int playerId;
    int gameId;
    uint8_t duck_id;
    Queue<Snapshot>& sender_q;
    std::atomic<bool>& is_alive;
    ServerSender sender;
    bool in_lobby;

    void handle_create_game();
    void handle_list_games() const;
    GameInfo handle_join_game();
    void wait_for_start() const;

public:
    ServerReceiver(ServerProtocol& protocol, GamesMonitor& games_monitor, Queue<Snapshot>& sender_q,
                   int playerId, std::atomic<bool>& is_alive);

    void setup_game();

    void run() override;

    void join_sender();

    ~ServerReceiver();
};

#endif

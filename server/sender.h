#ifndef SENDER_H
#define SENDER_H

#include "common/blocking_queue.h"
#include "common/snapshot.h"
#include "common/thread.h"

#include "server_protocol.h"

class ServerSender: public Thread {
private:
    ServerProtocol& protocol;
    Queue<Snapshot>& sender_q;
    int playerId;
    std::atomic<bool>& is_alive;

public:
    ServerSender(ServerProtocol& protocol, Queue<Snapshot>& sender_q, int playerId, std::atomic<bool>& is_alive);

    // void send_duck_id(const uint8_t& duck_id);
    void send_game_info(int32_t game_id, uint8_t duck_id);

    void run() override;

    ~ServerSender();

};

#endif

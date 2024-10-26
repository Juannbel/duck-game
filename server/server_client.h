#ifndef SERVER_CLIENT_H
#define SERVER_CLIENT_H

// TODO: Todos los includes necesarios
#include <vector>
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

    std::atomic<bool> is_alive;

public:
    ServerClient(Socket&& sk);
    Queue<Snapshot>& get_sender_queue() { return sender_q; }
    void set_gameloop_queue(Queue<struct action>* queue);
    void send_game_info(MatchInfo match_info);
    int receive_cmd();
    void send_games_info(std::vector<int> list_lobbies);
    void start();
    void join();
    bool is_dead();
    void kill();
};

#endif

#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include "common/commands.h"
#include "common/snapshot.h"
#include "common/socket.h"

class ServerProtocol {
private:
    Socket& socket;  // protocol is the socket owner.

public:
    explicit ServerProtocol(Socket& socket);

    void send_match_info(const MatchInfo& match_info);

    /*
    Definiciones provisorias por el momento.
    */
    void send_id_player(const uint8_t& duck_id);
    void send_map(void *map);

    void send_snapshot(const Snapshot& snapshot);

    Command recv_player_command();

private:
    Snapshot serializeSnapshot(const Snapshot& snapshot);

    ServerProtocol(const ServerProtocol&) = delete;
    ServerProtocol& operator=(const ServerProtocol&) = delete;

    ServerProtocol(ServerProtocol&& other) = delete;
    ServerProtocol& operator=(ServerProtocol&& other) = delete;
};
#endif

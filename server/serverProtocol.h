#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include "../common/socket.h"
#include "../common/snapshot.h"

class ServerProtocol {
private:
    Socket socket; // protocol is the socket owner.

public:
    explicit ServerProtocol(Socket socket);

    void send_game_info(const Snapshot& snapshot);

    void rec_player_command();

private:

    Snapshot serializeSnapshot(const Snapshot& snapshot);

    ServerProtocol(const ServerProtocol&) = delete;
    ServerProtocol& operator=(const ServerProtocol&) = delete;

    ServerProtocol(ServerProtocol&& other) = delete;
    ServerProtocol& operator=(ServerProtocol&& other) = delete;
};
#endif

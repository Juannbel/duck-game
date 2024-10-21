#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include "../common/socket.h"
#include "../common/snapshot.h"
#include "../common/commands.h"

class ClientProtocol {
private:
    Socket socket; // protocol is the socket owner.

public:
    explicit ClientProtocol(Socket &&socket);

    Snapshot recv_snapshot();

    void send_player_command(const Command& snapshot);

private:

    Snapshot deserializeSnapshot(const Snapshot& snapshot);

    ClientProtocol(const ClientProtocol&) = delete;
    ClientProtocol& operator=(const ClientProtocol&) = delete;

    ClientProtocol(ClientProtocol&& other) = delete;
    ClientProtocol& operator=(ClientProtocol&& other) = delete;
};
#endif

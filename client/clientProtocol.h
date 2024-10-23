#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include "../common/commands.h"
#include "../common/snapshot.h"
#include "../common/socket.h"

class ClientProtocol {
private:
    Socket socket;  // protocol is the socket owner.

public:
    explicit ClientProtocol(Socket&& socket);

    /*
    Definiciones provisorias por el momento.
    */
    void recv_id_player(const uint8_t& duck_id);
    void recv_map(void *map);

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

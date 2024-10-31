#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include <vector>

#include "common/commands.h"
#include "common/map_dto.h"
#include "common/snapshot.h"
#include "common/socket.h"

class ClientProtocol {
private:
    Socket socket;  // protocol is the socket owner.

public:
    explicit ClientProtocol(Socket&& socket);

    Snapshot recv_snapshot();
    uint8_t recv_duck_id();

    void send_player_command(const Command& snapshot);

    void send_option(int option);
    int recv_lobby();

    void shutdown();

private:
    Snapshot deserializeSnapshot(const Snapshot& snapshot);

    bool recv_match_finished(bool& was_closed);
    std::vector<Map> recv_maps_vector(const bool& match_finished, bool& wasClosed);
    std::vector<Duck> recv_ducks_vector(bool& was_closed);
    std::vector<Gun> recv_guns_vector(bool& was_closed);
    std::vector<Bullet> recv_bullets_vector(bool& was_closed);

    ClientProtocol(const ClientProtocol&) = delete;
    ClientProtocol& operator=(const ClientProtocol&) = delete;

    ClientProtocol(ClientProtocol&& other) = delete;
    ClientProtocol& operator=(ClientProtocol&& other) = delete;
};
#endif

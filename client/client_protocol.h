#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include "common/commands.h"
#include "common/map_dto.h"
#include "common/snapshot.h"
#include "common/socket.h"

#include <vector>

class ClientProtocol {
private:
    Socket socket;  // protocol is the socket owner.

public:
    explicit ClientProtocol(Socket&& socket);

    Snapshot recv_snapshot();
    MatchInfo recv_match_info();

    void send_player_command(const Command& snapshot);

    void shutdown();

private:
    Snapshot deserializeSnapshot(const Snapshot& snapshot);

    std::vector<Duck> recv_ducks_vector(bool &was_closed);
    std::vector<Gun> recv_guns_vector(bool &was_closed);
    std::vector<Bullet> recv_bullets_vector(bool &was_closed);

    ClientProtocol(const ClientProtocol&) = delete;
    ClientProtocol& operator=(const ClientProtocol&) = delete;

    ClientProtocol(ClientProtocol&& other) = delete;
    ClientProtocol& operator=(ClientProtocol&& other) = delete;
};
#endif

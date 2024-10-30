#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <vector>

#include "../common/map_dto.h"
#include "common/commands.h"
#include "common/snapshot.h"
#include "common/socket.h"

class ServerProtocol {
private:
    Socket& socket;  // protocol is the socket owner.

public:
    explicit ServerProtocol(Socket& socket);

    void send_match_info(const MatchInfo& match_info);

    void send_snapshot(const Snapshot& snapshot);

    int receive_cmd();
    void send_lobby_info(int lobby);


    Command recv_player_command();

private:
    Snapshot serializeSnapshot(const Snapshot& snapshot);

    template <typename T>
    void send_snapshot_vector(const std::vector<T>& vector, bool& wasClosed);

    void send_ducks_vector(const std::vector<Duck>& vector, bool& wasClosed);

    void send_guns_vector(const std::vector<Gun>& vector, bool& wasClosed);

    void send_bullets_vector(const std::vector<Bullet>& vector, bool& wasClosed);

    ServerProtocol(const ServerProtocol&) = delete;
    ServerProtocol& operator=(const ServerProtocol&) = delete;

    ServerProtocol(ServerProtocol&& other) = delete;
    ServerProtocol& operator=(ServerProtocol&& other) = delete;
};
#endif

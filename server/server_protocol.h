#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <vector>

#include "../common/map_dto.h"
#include "common/commands.h"
#include "common/lobby.h"
#include "common/snapshot.h"
#include "common/socket.h"

class ServerProtocol {
private:
    Socket& socket;  // protocol is the socket owner.

public:
    explicit ServerProtocol(Socket& socket);

    void send_duck_id(const uint8_t& duck_id);

    void send_snapshot(Snapshot& snapshot);

    int32_t receive_cmd();

    void send_lobbies_info(std::vector<int32_t>& lobbies);

    void send_game_info(GameInfo game_info);

    Command recv_player_command();

    void shutdown();

private:
    void serializeSnapshot(Snapshot& snapshot);

    void send_match_finished(const bool& match_finished, bool& was_closed);

    template <typename T>
    void send_snapshot_vector(const std::vector<T>& vector, bool& wasClosed);

    ServerProtocol(const ServerProtocol&) = delete;
    ServerProtocol& operator=(const ServerProtocol&) = delete;

    ServerProtocol(ServerProtocol&& other) = delete;
    ServerProtocol& operator=(ServerProtocol&& other) = delete;
};
#endif

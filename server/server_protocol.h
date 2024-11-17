#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <string>
#include <vector>

#include "common/commands.h"
#include "common/lobby.h"
#include "common/snapshot.h"
#include "common/socket.h"

class ServerProtocol {
private:
    Socket& socket;  // protocol is the socket owner.

public:
    explicit ServerProtocol(Socket& socket);

    void send_snapshot(Snapshot& snapshot);

    int32_t receive_cmd();

    std::string recv_string();

    void send_lobbies_info(std::vector<LobbyInfo>& lobbies);

    void send_game_info(GameInfo game_info);

    action recv_player_action();

    void send_option(int32_t option);

    void shutdown();

private:
    void serializeSnapshot(Snapshot& snapshot);

    void send_bool(const bool& b, bool& was_closed);

    template <typename T>
    void send_snapshot_vector(const std::vector<T>& vector, bool& wasClosed);

    ServerProtocol(const ServerProtocol&) = delete;
    ServerProtocol& operator=(const ServerProtocol&) = delete;

    ServerProtocol(ServerProtocol&& other) = delete;
    ServerProtocol& operator=(ServerProtocol&& other) = delete;
};
#endif

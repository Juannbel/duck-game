#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include <vector>

#include "common/commands.h"
#include "common/lobby.h"
#include "common/snapshot.h"
#include "common/socket.h"

class ClientProtocol {
private:
    Socket socket;  // protocol is the socket owner.

public:
    explicit ClientProtocol(Socket&& socket);

    Snapshot recv_snapshot();

    void send_player_action(const action& action);

    void send_option(int32_t option);

    void send_string(const std::string& str);

    GameInfo recv_game_info();

    std::vector<LobbyInfo> recv_lobbies_info();

    void shutdown();

private:
    void deserialize_snapshot(Snapshot& snapshot);

    bool recv_match_finished(bool& was_closed);

    template <typename T>
    void recv_vector(std::vector<T>& v, bool& was_closed);

    ClientProtocol(const ClientProtocol&) = delete;
    ClientProtocol& operator=(const ClientProtocol&) = delete;

    ClientProtocol(ClientProtocol&& other) = delete;
    ClientProtocol& operator=(ClientProtocol&& other) = delete;
};
#endif

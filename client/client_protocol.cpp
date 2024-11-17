#include "client_protocol.h"

#include <string>
#include <utility>
#include <vector>

#include <arpa/inet.h>
#include <sys/socket.h>


ClientProtocol::ClientProtocol(Socket&& socket): socket(std::move(socket)) {}

template <typename T>
void ClientProtocol::recv_vector(std::vector<T>& v, bool& was_closed) {
    uint8_t quantity;
    socket.recvall(&quantity, sizeof(quantity), &was_closed);
    if (was_closed)
        throw SocketWasClosed();

    v.resize(quantity);
    socket.recvall(v.data(), quantity * sizeof(T), &was_closed);
    if (was_closed)
        throw SocketWasClosed();
}

bool ClientProtocol::recv_bool(bool& was_closed) {
    bool b;
    socket.recvall(&b, sizeof(b), &was_closed);
    if (was_closed)
        throw SocketWasClosed();
    return b;
}

Snapshot ClientProtocol::recv_snapshot() {
    bool was_closed = false;
    Snapshot snapshot;

    snapshot.round_finished = recv_bool(was_closed);
    snapshot.show_stats = recv_bool(was_closed);
    snapshot.game_finished = recv_bool(was_closed);
    recv_vector<Duck>(snapshot.ducks, was_closed);
    recv_vector<Gun>(snapshot.guns, was_closed);
    recv_vector<Bullet>(snapshot.bullets, was_closed);
    recv_vector<Box>(snapshot.boxes, was_closed);
    recv_vector<MapDto>(snapshot.maps, was_closed);
    deserialize_snapshot(snapshot);

    return snapshot;
}

void ClientProtocol::deserialize_snapshot(Snapshot& snapshot) {

    int players_quantity = snapshot.ducks.size();
    for (int i = 0; i < players_quantity; i++) {
        Duck& duck = snapshot.ducks[i];
        duck.x = ntohs(duck.x);
        duck.y = ntohs(duck.y);
    }

    int guns_quantity = snapshot.guns.size();
    for (int i = 0; i < guns_quantity; i++) {
        Gun& gun = snapshot.guns[i];
        gun.gun_id = ntohl(gun.gun_id);
        gun.x = ntohs(gun.x);
        gun.y = ntohs(gun.y);
    }

    int bullets_quantity = snapshot.bullets.size();
    for (int i = 0; i < bullets_quantity; i++) {
        Bullet& bullet = snapshot.bullets[i];
        bullet.bullet_id = ntohl(bullet.bullet_id);
        bullet.x = ntohs(bullet.x);
        bullet.y = ntohs(bullet.y);
    }

    int boxes_quantity = snapshot.boxes.size();
    for (int i = 0; i < boxes_quantity; i++) {
        Box& box = snapshot.boxes[i];
        box.box_id = ntohl(box.box_id);
        box.x = ntohs(box.x);
        box.y = ntohs(box.y);
    }
}

void ClientProtocol::send_player_action(const action& action) {
    bool was_closed = false;
    socket.sendall(&action, sizeof(action), &was_closed);
    if (was_closed)
        throw SocketWasClosed();
}

void ClientProtocol::shutdown() {
    socket.shutdown(SHUT_RDWR);
    socket.close();
}

void ClientProtocol::send_option(int32_t option) {
    bool was_closed = false;
    option = htonl(option);
    socket.sendall(&option, sizeof(option), &was_closed);
    if (was_closed)
        throw SocketWasClosed();
}

int32_t ClientProtocol::recv_option() {
    bool was_closed = false;
    int32_t option;
    socket.recvall(&option, sizeof(option), &was_closed);
    if (was_closed)
        throw SocketWasClosed();
    return ntohl(option);
}

void ClientProtocol::send_string(const std::string& str) {
    bool was_closed = false;
    uint8_t str_len = str.size();
    socket.sendall(&str_len, sizeof(str_len), &was_closed);
    if (was_closed)
        throw SocketWasClosed();

    socket.sendall(str.c_str(), str_len, &was_closed);
    if (was_closed)
        throw SocketWasClosed();
}

GameInfo ClientProtocol::recv_game_info() {
    bool was_closed = false;
    GameInfo game_info;
    socket.recvall(&game_info, sizeof(game_info), &was_closed);
    if (was_closed)
        throw SocketWasClosed();
    game_info.game_id = ntohl(game_info.game_id);
    return game_info;
}

std::vector<LobbyInfo> ClientProtocol::recv_lobbies_info() {
    bool was_closed = false;
    uint8_t lobbies_quantity;
    socket.recvall(&lobbies_quantity, sizeof(lobbies_quantity), &was_closed);

    std::vector<LobbyInfo> lobbies_info(lobbies_quantity);
    socket.recvall(lobbies_info.data(), lobbies_quantity * sizeof(LobbyInfo), &was_closed);
    if (was_closed)
        throw SocketWasClosed();

    for (auto& lobby: lobbies_info) {
        lobby.game_id = ntohl(lobby.game_id);
    }

    return lobbies_info;
}

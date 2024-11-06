#include "server_protocol.h"

#include <cstdint>
#include <iostream>

#include <arpa/inet.h>

#include "common/commands.h"
#include "common/lobby.h"

ServerProtocol::ServerProtocol(Socket& socket): socket(socket) {}

template <typename T>
void ServerProtocol::send_snapshot_vector(const std::vector<T>& vector, bool& was_closed) {
    uint8_t vector_size = vector.size();
    socket.sendall(&vector_size, sizeof(vector_size), &was_closed);
    if (was_closed)
        throw SocketWasClosed();
    socket.sendall(vector.data(), vector_size * sizeof(T), &was_closed);
    if (was_closed)
        throw SocketWasClosed();
}

void ServerProtocol::send_match_finished(const bool& match_finished, bool& was_closed) {
    socket.sendall(&match_finished, sizeof(match_finished), &was_closed);
    if (was_closed)
        throw SocketWasClosed();
}

void ServerProtocol::send_snapshot(Snapshot& snapshot) {
    bool was_closed = false;
    serializeSnapshot(snapshot);

    send_match_finished(snapshot.match_finished, was_closed);
    send_snapshot_vector(snapshot.ducks, was_closed);
    send_snapshot_vector(snapshot.guns, was_closed);
    send_snapshot_vector(snapshot.bullets, was_closed);
    send_snapshot_vector(snapshot.boxes, was_closed);
    send_snapshot_vector(snapshot.maps, was_closed);
}

void ServerProtocol::send_duck_id(const uint8_t& duck_id) {
    bool was_closed = false;
    socket.sendall(&duck_id, sizeof(duck_id), &was_closed);
    if (was_closed)
        throw SocketWasClosed();
}

void ServerProtocol::serializeSnapshot(Snapshot& snapshot) {
    int players_quantity = snapshot.ducks.size();
    for (int i = 0; i < players_quantity; i++) {
        Duck& duck = snapshot.ducks[i];
        duck.x = htons(duck.x);
        duck.y = htons(duck.y);
    }

    int guns_quantity = snapshot.guns.size();
    for (int i = 0; i < guns_quantity; i++) {
        Gun& gun = snapshot.guns[i];
        gun.gun_id = htonl(gun.gun_id);
        gun.x = htons(gun.x);
        gun.y = htons(gun.y);
    }

    int bullets_quantity = snapshot.bullets.size();
    for (int i = 0; i < bullets_quantity; i++) {
        Bullet& bullet = snapshot.bullets[i];
        bullet.bullet_id = htonl(bullet.bullet_id);
        bullet.x = htons(bullet.x);
        bullet.y = htons(bullet.y);
    }

    int boxes_quantity = snapshot.boxes.size();
    for (int i = 0; i < boxes_quantity; i++) {
        Box& box = snapshot.boxes[i];
        box.x = htons(box.x);
        box.y = htons(box.y);
    }
}

Command ServerProtocol::recv_player_command() {
    bool was_closed = false;
    Command command;
    socket.recvall(&command, sizeof(command), &was_closed);
    if (was_closed)
        throw SocketWasClosed();
    return command;
}

int32_t ServerProtocol::receive_cmd() {
    bool wasClosed = false;
    int32_t command;
    socket.recvall(&command, sizeof(command), &wasClosed);
    if (wasClosed)
        throw SocketWasClosed();
    return ntohl(command);
}

void ServerProtocol::send_lobbies_info(std::vector<int32_t>& lobbies) {
    uint8_t lobbies_quantity = lobbies.size();

    for (int i = 0; i < lobbies_quantity; i++) {
        lobbies[i] = htonl(lobbies[i]);
    }

    bool wasClosed = false;
    socket.sendall(&lobbies_quantity, sizeof(lobbies_quantity), &wasClosed);
    socket.sendall(lobbies.data(), lobbies_quantity * sizeof(int32_t), &wasClosed);

    if (wasClosed)
        throw SocketWasClosed();
}

void ServerProtocol::send_game_info(GameInfo game_info) {
    game_info.game_id = htonl(game_info.game_id);

    bool wasClosed = false;
    socket.sendall(&game_info, sizeof(game_info), &wasClosed);
    if (wasClosed)
        throw SocketWasClosed();
}


void ServerProtocol::shutdown() {
    socket.shutdown(SHUT_RDWR);
    socket.close();
}

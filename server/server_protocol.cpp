#include "server_protocol.h"

#include <cstdint>
#include <iostream>

#include <arpa/inet.h>

#include "common/commands.h"
#include "common/lobby.h"

ServerProtocol::ServerProtocol(Socket& socket): socket(socket) {}

template <typename T>
void ServerProtocol::send_snapshot_vector(const std::vector<T>& vector, bool& was_closed) {
    uint8_t vectorSize = vector.size();
    socket.sendall(&vectorSize, sizeof(vectorSize), &was_closed);
    if (was_closed)
        throw SocketWasClosed();
    socket.sendall(vector.data(), vectorSize * sizeof(T), &was_closed);
    if (was_closed)
        throw SocketWasClosed();
}

void ServerProtocol::send_ducks_vector(const std::vector<Duck>& ducks, bool& was_closed) {
    uint8_t players_quantity = ducks.size();
    socket.sendall(&players_quantity, sizeof(players_quantity), &was_closed);
    if (was_closed)
        throw SocketWasClosed();
    socket.sendall(ducks.data(), players_quantity * sizeof(Duck), &was_closed);
    if (was_closed)
        throw SocketWasClosed();
}

void ServerProtocol::send_guns_vector(const std::vector<Gun>& guns, bool& was_closed) {
    uint8_t guns_quantity = guns.size();
    socket.sendall(&guns_quantity, sizeof(guns_quantity), &was_closed);
    if (was_closed)
        throw SocketWasClosed();
    socket.sendall(guns.data(), guns_quantity * sizeof(Gun), &was_closed);
    if (was_closed)
        throw SocketWasClosed();
}

void ServerProtocol::send_bullets_vector(const std::vector<Bullet>& bullets, bool& was_closed) {
    uint8_t bullets_quantity = bullets.size();
    socket.sendall(&bullets_quantity, sizeof(bullets_quantity), &was_closed);
    if (was_closed)
        throw SocketWasClosed();
    socket.sendall(bullets.data(), bullets_quantity * sizeof(Bullet), &was_closed);
    if (was_closed)
        throw SocketWasClosed();
}

void ServerProtocol::send_match_finished(const bool& match_finished, bool& was_closed) {
    socket.sendall(&match_finished, sizeof(match_finished), &was_closed);
    if (was_closed)
        throw SocketWasClosed();
}

void ServerProtocol::send_maps_vector(const std::vector<Map>& maps, bool& was_closed) {
    uint8_t map_quantity = maps.size();
    socket.sendall(&map_quantity, sizeof(map_quantity), &was_closed);
    if (was_closed)
        throw SocketWasClosed();
    socket.sendall(maps.data(), map_quantity * sizeof(Map), &was_closed);
    if (was_closed)
        throw SocketWasClosed();
}

void ServerProtocol::send_snapshot(const Snapshot& snapshot) {
    bool was_closed = false;
    Snapshot serializedSS = serializeSnapshot(snapshot);

    send_match_finished(serializedSS.match_finished, was_closed);
    send_ducks_vector(serializedSS.ducks, was_closed);
    send_guns_vector(serializedSS.guns, was_closed);
    send_bullets_vector(serializedSS.bullets, was_closed);
    send_maps_vector(serializedSS.maps, was_closed);
}

void ServerProtocol::send_duck_id(const uint8_t& duck_id) {
    bool was_closed = false;
    socket.sendall(&duck_id, sizeof(duck_id), &was_closed);
    if (was_closed)
        throw SocketWasClosed();
}

Snapshot ServerProtocol::serializeSnapshot(const Snapshot& snapshot) {
    Snapshot serializedSS(snapshot);

    int players_quantity = serializedSS.ducks.size();
    for (int i = 0; i < players_quantity; i++) {
        Duck& duck = serializedSS.ducks[i];
        duck.x = htons(duck.x);
        duck.y = htons(duck.y);
    }

    int guns_quantity = serializedSS.guns.size();
    for (int i = 0; i < guns_quantity; i++) {
        Gun& gun = serializedSS.guns[i];
        gun.gun_id = htonl(gun.gun_id);
        gun.x = htons(gun.x);
        gun.y = htons(gun.y);
    }

    int bullets_quantity = serializedSS.bullets.size();
    for (int i = 0; i < bullets_quantity; i++) {
        Bullet& bullet = serializedSS.bullets[i];
        bullet.bullet_id = htonl(bullet.bullet_id);
        bullet.x = htons(bullet.x);
        bullet.y = htons(bullet.y);
    }
    return serializedSS;
}

Command ServerProtocol::recv_player_command() {
    bool was_closed = false;
    Command command;
    socket.recvall(&command, sizeof(command), &was_closed);
    if (was_closed)
        throw SocketWasClosed();
    return command;
}

// Cambiar endianness y tamaño definido (int32_t) (con command)
int32_t ServerProtocol::receive_cmd() {
    bool wasClosed = false;
    int32_t command;
    socket.recvall(&command, sizeof(command), &wasClosed);
    if (wasClosed)
        throw SocketWasClosed();
    return ntohl(command);
}

void ServerProtocol::send_lobbies_info(std::vector<int32_t> &lobbies) {
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

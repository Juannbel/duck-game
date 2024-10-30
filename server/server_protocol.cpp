#include "server_protocol.h"

#include <arpa/inet.h>

#include "common/commands.h"

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
    socket.sendall(&maps, sizeof(Map), &was_closed);
    if (was_closed)
        throw SocketWasClosed();
}

void ServerProtocol::send_snapshot(const Snapshot& snapshot) {
    bool was_closed = false;
    Snapshot serializedSS = serializeSnapshot(snapshot);

    send_ducks_vector(serializedSS.ducks, was_closed);
    send_guns_vector(serializedSS.guns, was_closed);
    send_bullets_vector(serializedSS.bullets, was_closed);
    send_match_finished(serializedSS.match_finished, was_closed);
    if (serializedSS.match_finished) {
        send_maps_vector(serializedSS.maps, was_closed);
    }
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


void ServerProtocol::shutdown() {
    socket.shutdown(SHUT_RDWR);
    socket.close();
}

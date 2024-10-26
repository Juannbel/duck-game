#include "client_protocol.h"

#include <stdexcept>
#include <utility>

#include <arpa/inet.h>
#include <sys/socket.h>
#include "common/map_dto.h"

ClientProtocol::ClientProtocol(Socket&& socket): socket(std::move(socket)) {}

std::vector<Duck> ClientProtocol::recv_ducks_vector(bool &was_closed){
    uint8_t players_quantity;
    socket.recvall(&players_quantity, sizeof(players_quantity), &was_closed);
    std::vector<Duck> ducks(players_quantity);
    socket.recvall(ducks.data(), players_quantity * sizeof(Duck), &was_closed);
    return ducks;
}

std::vector<Gun> ClientProtocol::recv_guns_vector(bool &was_closed){
    uint8_t guns_quantity;
    socket.recvall(&guns_quantity, sizeof(guns_quantity), &was_closed);
    std::vector<Gun> guns(guns_quantity);
    socket.recvall(guns.data(), guns_quantity * sizeof(Gun), &was_closed);
    return guns;
}

std::vector<Bullet> ClientProtocol::recv_bullets_vector(bool &was_closed){
    uint8_t bullets_quantity;
    socket.recvall(&bullets_quantity, sizeof(bullets_quantity), &was_closed);
    std::vector<Bullet> bullets(bullets_quantity);
    socket.recvall(bullets.data(), bullets_quantity * sizeof(Bullet), &was_closed);
    return bullets;
}

Snapshot ClientProtocol::recv_snapshot() {
    bool was_closed;
    Snapshot serializedSnapshot;

    serializedSnapshot.ducks = recv_ducks_vector(was_closed);
    serializedSnapshot.guns =recv_guns_vector(was_closed);
    serializedSnapshot.bullets = recv_bullets_vector(was_closed);

    Snapshot deserializedSnapshot = deserializeSnapshot(serializedSnapshot);

    return deserializedSnapshot;
}

Snapshot ClientProtocol::deserializeSnapshot(const Snapshot& snapshot) {
    Snapshot deserializedSS(snapshot);

    int players_quantity = deserializedSS.ducks.size();
    for (int i = 0; i < players_quantity; i++) {
        Duck& duck = deserializedSS.ducks[i];
        duck.x = ntohs(duck.x);
        duck.y = ntohs(duck.y);
    }

    int guns_quantity = deserializedSS.guns.size();
    for (int i = 0; i < guns_quantity; i++) {
        Gun& gun = deserializedSS.guns[i];
        gun.gun_id = ntohl(gun.gun_id);
        gun.x = ntohs(gun.x);
        gun.y = ntohs(gun.y);
    }

    int bullets_quantity = deserializedSS.bullets.size();
    for (int i = 0; i < bullets_quantity; i++) {
        Bullet& bullet = deserializedSS.bullets[i];
        bullet.bullet_id = ntohl(bullet.bullet_id);
        bullet.x = ntohs(bullet.x);
        bullet.y = ntohs(bullet.y);
    }
    return deserializedSS;
}

void ClientProtocol::send_player_command(const Command& command) {
    bool wasClosed = false;
    socket.sendall(&command, sizeof(command), &wasClosed);
    // excepción.
}

MatchInfo ClientProtocol::recv_match_info() {
    bool was_closed;
    MatchInfo match_info;
    socket.recvall(&match_info, sizeof(match_info), &was_closed);

    return match_info;
}

void ClientProtocol::shutdown() {
    socket.shutdown(SHUT_RDWR);
    socket.close();
}

void ClientProtocol::send_option(int option) {
    bool was_closed = false;
    socket.sendall(&option, sizeof(option), &was_closed);
}

int ClientProtocol::recv_lobby() {
    bool was_closed = false;
    int id;
    // TODO: Modificar protocolo
    // (protocolo->envio -1 al final desde server) por ahora solo id's que son int
    socket.recvall(&id, sizeof(id), &was_closed);
    return id;
}

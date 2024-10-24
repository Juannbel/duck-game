#include "client_protocol.h"

#include <stdexcept>
#include <utility>

#include <arpa/inet.h>
#include <sys/socket.h>
#include "common/map_dto.h"

ClientProtocol::ClientProtocol(Socket&& socket): socket(std::move(socket)) {}

void ClientProtocol::recv_id_player(const uint8_t& duck_id){}

void ClientProtocol::recv_map(void *map){}

Snapshot ClientProtocol::recv_snapshot() {
    bool was_closed;
    Snapshot serializedSnaphot;
    socket.recvall(&serializedSnaphot, sizeof(Snapshot), &was_closed);

    Snapshot deserializedSnapshot = deserializeSnapshot(serializedSnaphot);
    return deserializedSnapshot;
}

Snapshot ClientProtocol::deserializeSnapshot(const Snapshot& snapshot) {
    Snapshot deserializedSS(snapshot);
    for (int i = 0; i < deserializedSS.players_quantity; i++) {
        Duck& duck = deserializedSS.ducks[i];
        duck.x = ntohs(duck.x);
        duck.y = ntohs(duck.y);
    }

    for (int i = 0; i < deserializedSS.guns_quantity; i++) {
        Gun& gun = deserializedSS.guns[i];
        gun.gun_id = ntohl(gun.gun_id);
        gun.x = ntohs(gun.x);
        gun.y = ntohs(gun.y);
    }

    for (int i = 0; i < deserializedSS.bullets_quantity; i++) {
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

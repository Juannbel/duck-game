#include "server_protocol.h"

#include <arpa/inet.h>

#include "common/commands.h"

ServerProtocol::ServerProtocol(Socket& socket): socket(socket) {}

template <typename T>
void ServerProtocol::send_snapshot_vector(const std::vector<T>& vector, bool &wasClosed){
    uint8_t vectorSize = vector.size(); // MAX_DUCK is 4. Not problem with hexadecimal conversion.
    socket.sendall(&vectorSize, sizeof(vectorSize), &wasClosed);
    socket.sendall(vector.data(), vectorSize * sizeof(T), &wasClosed);
}

void ServerProtocol::send_ducks_vector(const std::vector<Duck>& ducks, bool &wasClosed){
    uint8_t players_quantity = ducks.size(); // MAX_DUCK is 4. Not problem with hexadecimal conversion.
    socket.sendall(&players_quantity, sizeof(players_quantity), &wasClosed);
    socket.sendall(ducks.data(), players_quantity * sizeof(Duck), &wasClosed);
}

void ServerProtocol::send_guns_vector(const std::vector<Gun>& guns, bool &wasClosed){
    uint8_t guns_quantity = guns.size(); // MAX_DUCK is 4. Not problem with hexadecimal conversion.
    socket.sendall(&guns_quantity, sizeof(guns_quantity), &wasClosed);
    socket.sendall(guns.data(), guns_quantity * sizeof(Gun), &wasClosed);
}

void ServerProtocol::send_bullets_vector(const std::vector<Bullet>& bullets, bool &wasClosed){
    uint8_t bullets_quantity = bullets.size(); // MAX_DUCK is 4. Not problem with hexadecimal conversion.
    socket.sendall(&bullets_quantity, sizeof(bullets_quantity), &wasClosed);
    socket.sendall(bullets.data(), bullets_quantity * sizeof(Bullet), &wasClosed);
}

void ServerProtocol::send_snapshot(const Snapshot& snapshot) {
    bool wasClosed = false;
    Snapshot serializedSS = serializeSnapshot(snapshot);

    send_ducks_vector(serializedSS.ducks, wasClosed);
    send_guns_vector(serializedSS.guns, wasClosed);
    send_bullets_vector(serializedSS.bullets, wasClosed);

 
}

void ServerProtocol::send_match_info(const MatchInfo& matchInfo) {
    bool wasClosed = false;
    socket.sendall(&matchInfo, sizeof(matchInfo), &wasClosed);
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
    bool wasClosed = false;
    Command command;
    socket.recvall(&command, sizeof(command), &wasClosed);
    // excepción.
    return command;
}

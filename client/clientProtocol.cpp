#include "clientProtocol.h"
#include <utility>
#include <arpa/inet.h>

ClientProtocol::ClientProtocol(Socket socket):
                            socket(std::move(socket)){}

Snapshot ClientProtocol::rec_snapshot(const Snapshot& snapshot){
    Snapshot deserializedSnapshot = deserializeSnapshot(snapshot);
    return deserializedSnapshot;
}

Snapshot ClientProtocol::deserializeSnapshot(const Snapshot& snapshot){
    Snapshot deserializedSS(snapshot);
    for(int i = 0; i < deserializedSS.players_quantity;i++){
        Duck duck = deserializedSS.ducks[i];
        duck.x = ntohl(duck.x);
        duck.y = ntohl(duck.y);
    }

    for(int i = 0; i < deserializedSS.guns_quantity ; i++ ){
        Gun gun = deserializedSS.guns[i];
        gun.gun_id = ntohl(gun.gun_id);
        gun.x = ntohl(gun.x);
        gun.y = ntohl(gun.y);
    }

    for(int i = 0; i < deserializedSS.bullets_quantity; i++ ){
        Bullet bullet = deserializedSS.bullets[i];
        bullet.bullet_id = ntohl(bullet.bullet_id);
        bullet.x = ntohl(bullet.x);
        bullet.y = ntohl(bullet.y);
    }
    return deserializedSS;
}

void ClientProtocol::send_player_command(const Command& command){
    bool wasClosed = false;
    socket.sendall(&command, sizeof(command),&wasClosed);
    //excepción.
}

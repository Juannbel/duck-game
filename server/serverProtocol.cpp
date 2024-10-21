#include "serverProtocol.h"

#include <utility>
#include <arpa/inet.h>
#include "common/commands.h"

ServerProtocol::ServerProtocol(Socket &socket):
                        socket(socket)
{}

void ServerProtocol::send_snapshot(const Snapshot& snapshot){
    bool wasClosed = false; //harcodeo por el momento
    Snapshot serializedSS = serializeSnapshot(snapshot);
    socket.sendall(&serializedSS,sizeof(serializedSS),&wasClosed);
    //lanzar excepcion si was closed fue cerrada, esto nos ayudara a stopear el protocolo.
}

Snapshot ServerProtocol::serializeSnapshot(const Snapshot& snapshot){
    Snapshot serializedSS(snapshot);

    for(int i = 0; i < serializedSS.players_quantity;i++){
        Duck &duck = serializedSS.ducks[i];
        duck.x = htons(duck.x);
        duck.y = htons(duck.y);
    }

    for(int i = 0; i < serializedSS.guns_quantity ; i++ ){
        Gun &gun = serializedSS.guns[i];
        gun.gun_id = htonl(gun.gun_id);
        gun.x = htons(gun.x);
        gun.y = htons(gun.y);
    }

    for(int i = 0; i < serializedSS.bullets_quantity; i++ ){
        Bullet &bullet = serializedSS.bullets[i];
        bullet.bullet_id = htonl(bullet.bullet_id);
        bullet.x = htons(bullet.x);
        bullet.y = htons(bullet.y);
    }
    return serializedSS;
}

Command ServerProtocol::rec_player_command(){
    bool wasClosed = false;
    Command command; //Provisorio. aca seria tipo action, de rama fede.
    socket.recvall(&command, sizeof(command),&wasClosed);
    //excepción.
    return command;
}

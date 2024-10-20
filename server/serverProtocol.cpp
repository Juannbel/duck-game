#include "serverProtocol.h"

#include <utility>
#include <netinet/in.h>
#include "../common/commands.h"

explicit ServerProtocol::ServerProtocol(Socket socket):
                        socket(std::move(socket))
{}

void ServerProtocol::send_game_info(const Snapshot& snapshot){
    bool wasClosed = false; //harcodeo por el momento
    Snapshot serializedSS = serializeSnapshot(snapshot);
    socket.sendall(&serializedSS,sizeof(serializedSS),&wasClosed);
    //lanzar excepcion si was closed fue cerrada, esto nos ayudara a stopear el protocolo.
}

Snapshot ServerProtocol::serializeSnapshot(const Snapshot& snapshot){
    Snapshot serializedSS(snapshot);

    for(int i = 0; i < serializedSS.players_quantity;i++){
        Duck duck = serializedSS.ducks[i];
        duck.x = ntohl(duck.x);
        duck.y = ntohl(duck.y);
    }

    for(int i = 0; i < serializedSS.guns_quantity ; i++ ){
        Gun gun = serializedSS.guns[i];
        gun.gun_id = ntohl(gun.gun_id);
        gun.x = ntohl(gun.x);
        gun.y = ntohl(gun.y);
    }

    for(int i = 0; i < serializedSS.bullets_quantity; i++ ){
        Bullet bullet = serializedSS.bullets[i];
        bullet.bullet_id = ntohl(bullet.bullet_id);
        bullet.x = ntohl(bullet.x);
        bullet.y = ntohl(bullet.y);
    }
    return serializedSS;
}

void ServerProtocol::rec_player_command(){
    bool wasClosed = false;
    Command command; //Provisorio. aca seria tipo action, de rama fede.
    socket.recvall(&command, sizeof(command),&wasClosed);
    //excepción.
}

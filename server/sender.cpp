#include "sender.h"

#include <cstdint>
#include <iostream>

#include "common/liberror.h"
#include "common/lobby.h"
#include "common/snapshot.h"

ServerSender::ServerSender(ServerProtocol& protocol, Queue<Snapshot>& sender_q, int playerId,
                           std::atomic<bool>& is_alive):
        protocol(protocol), sender_q(sender_q), playerId(playerId), is_alive(is_alive) {}

void ServerSender::run() {
    while (_keep_running && is_alive) {
        Snapshot msg;
        // Espero que queue tenga algo y mando
        try {
            msg = sender_q.pop();  // bloqueante, espera a que haya algo
        } catch (const ClosedQueue&) {
            // Se cierra la queue cuando: 1. Se cierra el socket, 2. El cliente se desconecto
            // std::cout << "Closed queue en sender player id: " << playerId << std::endl;
            break;
        }

        try {
            // Envio el msg recibido en la queue
            protocol.send_snapshot(msg);
        } catch (const SocketWasClosed& e) {
            // Se cierra el socket cuando el cliente se desconecta
            break;
        } catch (const LibError& le) {
            std::cout << "LibError en sender player id: " << playerId << " " << le.what()
                      << std::endl;
            break;
        }
    }
    is_alive = false;
}

ServerSender::~ServerSender() {
    is_alive = false;
}

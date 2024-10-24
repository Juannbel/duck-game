#include "sender.h"
#include <iostream>

#include "common/liberror.h"
#include "common/snapshot.h"

ServerSender::ServerSender(ServerProtocol& protocol, Queue<Snapshot>& sender_q, uint8_t duck_id):
        protocol(protocol), sender_q(sender_q), duck_id(duck_id) {}

void ServerSender::run() {
    while (_keep_running) {
        Snapshot msg;
        // Espero que queue tenga algo y mando
        try {
            msg = sender_q.pop();  // bloqueante, espera a que haya algo
        } catch (const ClosedQueue&) {
            std::cout << "Closed queue en sender id: " << (int) duck_id << std::endl;
            break;
        }

        try {
            // Envio el msg recibido en la queue
            protocol.send_snapshot(msg);
        } catch (const LibError& le) { // Catchear excepcion de socket cerrado
            std::cout << "LibError en sender id: " << (int) duck_id << " " << le.what() << std::endl;
        }
    }
}

#include "sender.h"

#include "common/snapshot.h"

Sender::Sender(ServerProtocol& protocolo, Queue<Snapshot>& sender_q):
        protocolo(protocolo), sender_q(sender_q) {}

void Sender::run() {
    while (true) {
        // Espero que queue tenga algo y mando
        try {
            Snapshot msg = sender_q.pop();  // bloqueante, espera a que haya algo

            // Envio el msg recibido en la queue
            protocolo.send_snapshot(msg);

            // Catchear la excepcion de que el skt estaba cerrado
        } catch (const ClosedQueue&) {
            break;
        }
    }
}

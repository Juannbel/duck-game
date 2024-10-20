#include "sender.h"

#include "common/snapshot.h"

Sender::Sender(ServerProtocolo& protocolo, Queue<Snapshot>& sender_q):
        protocolo(protocolo), sender_q(sender_q) {}

void Sender::run() {
    bool was_closed = false;
    while (true) {
        // Espero que queue tenga algo y mando
        try {
            Snapshot msg = sender_q.pop();  // bloqueante, espera a que haya algo

            // Envio el msg recibido en la queue
            protocolo.enviar_msg(msg, was_closed);

            if (was_closed) {
                break;
            }
        } catch (const ClosedQueue&) {
            break;
        }
    }
}

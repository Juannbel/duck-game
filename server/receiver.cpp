#include "receiver.h"

Receiver::Receiver(ServerProtocolo& protocolo, Queue<Snapshot>& q):
        protocolo(protocolo), gameloop_q(q) {}

// Me quedo trabado en recibir_msg (hasta tener algo) y lo mando a queue de gameloop
void Receiver::run() {
    bool was_closed = false;
    // Snapshot msg;

    while (true) {
        protocolo.recv_msg(was_closed, msg);

        if (was_closed) {
            break;
        }

        gameloop_q.push(msg);
    }
}

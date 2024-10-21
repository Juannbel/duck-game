#include "receiver.h"
#include "action.h"

Receiver::Receiver(ServerProtocol& protocolo, Queue<struct action>& q):
        protocolo(protocolo), gameloop_q(q) {}

// Me quedo trabado en recibir_msg (hasta tener algo) y lo mando a queue de gameloop
void Receiver::run() {
    bool was_closed = false;
    // Snapshot msg;

    while (true) {
        Command cmd = protocolo.rec_player_command();
        // Catchear excepcion de 
        if (was_closed) {
            break;
        }
        struct action action;
        action.duck_id = 0;  // Agregar el n de pato
        action.command = cmd;

        gameloop_q.push(action);
    }
}

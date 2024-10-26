#include "receiver.h"

#include <iostream>

#include "common/blocking_queue.h"
#include "common/commands.h"
#include "common/liberror.h"

#include "action.h"

ServerReceiver::ServerReceiver(ServerProtocol& protocol):
        protocol(protocol) {}

// Me quedo trabado en recibir_msg (hasta tener algo) y lo mando a queue de gameloop
void ServerReceiver::run() {
    // bool was_closed = false; // comento hasta que se use por cppcheck
    // Snapshot msg;
    while (_keep_running) {
        Command cmd;
        try {
            cmd = protocol.recv_player_command();
        } catch (const LibError& le) {  // Catchear excepcion de socket cerrado
            std::cout << "LibError en receiver id: " << (int)duck_id << " " << le.what()
                      << std::endl;
        }

        struct action action;
        action.duck_id = duck_id;  // Agregar el n de pato
        action.command = cmd;

        try {
            gameloop_q->push(action);
        } catch (const ClosedQueue& e) {
            std::cout << "ClosedQueue en receiver id: " << (int)duck_id << " " << e.what()
                      << std::endl;
        }
    }
}

void ServerReceiver::set_gameloop_queue(Queue<struct action>* queue) {
    gameloop_q = queue;
}

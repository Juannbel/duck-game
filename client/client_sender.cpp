#include "client_sender.h"

#include "common/blocking_queue.h"
#include "common/liberror.h"

ClientSender::ClientSender(ClientProtocol& protocol, Queue<Command>& sender_q,
                           std::atomic<bool>& alive):
        protocol(protocol), sender_q(sender_q), alive(alive) {}

void ClientSender::run() {
    while (alive) {
        // Espero que queue tenga algo y mando
        Command command;
        try {
            command = sender_q.pop();  // bloqueante, espera a que haya algo
        } catch (const ClosedQueue&) {
            // std::cout << "closed queue en sender, todo bien, es para salir" << std::endl;
            break;
        }

        try {
            // Envio el command recibido en la queue
            protocol.send_player_command(command);
        } catch (const LibError& le) {
            // std::cout << "liberror en sender, todo bien, es para salir" << std::endl;
            break;
        } catch (const SocketWasClosed& se) {
            break;
        }
    }

    try {
        sender_q.close();
    } catch (...) {}
    alive = false;
}

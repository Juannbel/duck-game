#include "client_sender.h"


ClientSender::ClientSender(ClientProtocol& protocol, Queue<Command>& sender_q):
        protocol(protocol), sender_q(sender_q) {}

void ClientSender::run() {
    while (_keep_running) {
        // Espero que queue tenga algo y mando
        try {
            Command command = sender_q.pop();  // bloqueante, espera a que haya algo

            // Envio el command recibido en la queue
            protocol.send_player_command(command);

            // Catchear la excepcion de que el skt estaba cerrado
        } catch (const ClosedQueue&) {
            break;
        }
    }
}

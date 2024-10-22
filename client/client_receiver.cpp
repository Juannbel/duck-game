#include "client_receiver.h"

ClientReceiver::ClientReceiver(ClientProtocol& protocol, Queue<Snapshot>& q):
        protocol(protocol), snapshot_q(q) {}

void ClientReceiver::run() {
    while (_keep_running) {
        // Espero que queue tenga algo y mando
        try {

            Snapshot snapshot = protocol.recv_snapshot();

            // Envio el Snapshot recibido en la queue
            snapshot_q.push(snapshot);  // bloqueante, espera a que haya algo

            // Catchear la excepcion de que el skt estaba cerrado
        } catch (const ClosedQueue&) {
            break;
        }
    }
}

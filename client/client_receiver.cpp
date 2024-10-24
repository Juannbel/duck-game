#include "client_receiver.h"
#include <iostream>
#include "common/liberror.h"
#include "common/snapshot.h"

ClientReceiver::ClientReceiver(ClientProtocol& protocol, Queue<Snapshot>& q):
        protocol(protocol), snapshot_q(q) {}

void ClientReceiver::run() {
    while (_keep_running) {
        // Espero que queue tenga algo y mando
        Snapshot snapshot;
        try {
            snapshot = protocol.recv_snapshot();
        } catch (const LibError& le) {
            // Catchear la excepcion de que el skt estaba cerrado
            std::cout << "Receiver liberror " << le.what() << std::endl;
            break;
        }

        try {
            snapshot_q.push(snapshot);  // bloqueante, espera a que haya algo
        } catch (const ClosedQueue&) {
            std::cout << "Receiver closed queue " << std::endl;
            break;
        }
    }
}

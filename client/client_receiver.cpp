#include "client_receiver.h"

#include <atomic>

#include "common/liberror.h"
#include "common/snapshot.h"
#include "common/socket.h"

ClientReceiver::ClientReceiver(ClientProtocol& protocol, Queue<Snapshot>& q,
                               std::atomic<bool>& alive):
        protocol(protocol), snapshot_q(q), alive(alive) {}

void ClientReceiver::run() {
    while (alive) {
        // Espero que queue tenga algo y mando
        Snapshot snapshot;
        try {
            snapshot = protocol.recv_snapshot();
        } catch (const LibError& le) {
            // Catchear la excepcion de que el skt estaba cerrado
            // std::cout << "liberror en receiver, todo bien, es para salir" << std::endl;
            break;
        } catch (const SocketWasClosed& se) {
            break;
        }

        try {
            snapshot_q.push(snapshot);  // bloqueante, espera a que haya algo
        } catch (const ClosedQueue&) {
            // std::cout << "closed queue en receiver, todo bien, es para salir" << std::endl;
            break;
        }
    }

    try {
        snapshot_q.close();
    } catch (...) {}
    alive = false;
}

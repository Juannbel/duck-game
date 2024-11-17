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
        Snapshot snapshot;
        try {
            snapshot = protocol.recv_snapshot();
        } catch (const LibError& le) {
            break;
        } catch (const SocketWasClosed& se) {
            break;
        }

        try {
            snapshot_q.push(snapshot);
        } catch (const ClosedQueue&) {
            break;
        }
    }

    try {
        snapshot_q.close();
    } catch (...) {}
    alive = false;
}

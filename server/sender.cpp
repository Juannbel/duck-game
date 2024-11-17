#include "sender.h"

#include "common/liberror.h"
#include "common/snapshot.h"

ServerSender::ServerSender(ServerProtocol& protocol, Queue<Snapshot>& sender_q, int playerId,
                           std::atomic<bool>& is_alive):
        protocol(protocol), sender_q(sender_q), playerId(playerId), is_alive(is_alive) {}

void ServerSender::run() {
    while (_keep_running && is_alive) {
        Snapshot msg;
        try {
            msg = sender_q.pop();
        } catch (const ClosedQueue&) {
            break;
        }

        try {
            protocol.send_snapshot(msg);
        } catch (const SocketWasClosed& e) {
            break;
        } catch (const LibError& le) {
            break;
        }
    }
    is_alive = false;
}

ServerSender::~ServerSender() { is_alive = false; }

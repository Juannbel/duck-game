#include "client_sender.h"

#include "common/blocking_queue.h"
#include "common/liberror.h"

ClientSender::ClientSender(ClientProtocol& protocol, Queue<action>& sender_q,
                           std::atomic<bool>& alive):
        protocol(protocol), sender_q(sender_q), alive(alive) {}

void ClientSender::run() {
    while (alive) {
        action action;
        try {
            action = sender_q.pop();
        } catch (const ClosedQueue&) {
            break;
        }

        try {
            protocol.send_player_action(action);
        } catch (const LibError& le) {
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

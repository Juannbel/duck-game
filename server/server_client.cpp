#include "server_client.h"

#include <utility>

#include <sys/socket.h>

#include "common/blocking_queue.h"
#include "common/liberror.h"

#define SIZE_QUEUE 0

ServerClient::ServerClient(Socket&& sk, GamesMonitor& games_monitor, int id):
        sk(std::move(sk)),
        sender_q(Queue<Snapshot>(SIZE_QUEUE)),
        protocolo(ServerProtocol(this->sk)),
        receiver(protocolo, games_monitor, sender_q, id, is_alive),
        is_alive(true) {}

/* Inicializo los threads sender y receiver, para establecer comunicacion con el cliente */
void ServerClient::start() {
    is_alive = true;
    receiver.start();
}

void ServerClient::join() {
    // Cierro los threads
    receiver.join();
    receiver.join_sender();
    is_alive = false;
}

bool ServerClient::is_dead() { return not is_alive; }

Queue<Snapshot>* ServerClient::get_sender_queue() { return &sender_q; }

void ServerClient::kill() {
    try {
        sender_q.close();
    } catch (...) {}
    try {
        sk.shutdown(SHUT_RDWR);
    } catch (const LibError& le) {
        // evita Transport endpoint is not connected
    }
    sk.close();
}

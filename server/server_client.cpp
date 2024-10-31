#include "server_client.h"

#include <utility>
#include "common/blocking_queue.h"

#define SIZE_QUEUE 0

ServerClient::ServerClient(Socket&& sk, GamesMonitor& games_monitor, int id):
        sk(std::move(sk)),
        sender_q(Queue<Snapshot>(SIZE_QUEUE)),
        protocolo(ServerProtocol(this->sk)),
        receiver(protocolo, games_monitor, sender_q, id) {}

/* Inicializo los threads sender y receiver, para establecer comunicacion con el cliente */
void ServerClient::start() {
    is_alive = true;
    receiver.start();
}

void ServerClient::join() {
    // Cierro los threads
    receiver.join();
    is_alive = false;
}

bool ServerClient::is_dead() { return not is_alive; }

void ServerClient::kill() {
    sender_q.close();
    sk.shutdown(2);
    sk.close();
}

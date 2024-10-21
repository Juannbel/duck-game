#include "ServerClient.h"

#include <cstdio>
#include <utility>

#include <netinet/in.h>

#include "common/blocking_queue.h"

#define SIZE_QUEUE 100

ServerClient::ServerClient(Socket sk, Queue<struct action>& gameloop_q, int id):
        sk(std::move(sk)),
        sender_q(Queue<Snapshot>(SIZE_QUEUE)),
        protocolo(ServerProtocol(this->sk)),
        sender(protocolo, sender_q),
        receiver(protocolo, gameloop_q),
        id(id) {}

/* Inicializo los threads sender y receiver, para establecer comunicacion con el cliente */
void ServerClient::start() {
    is_alive = true;

    sender.start();
    receiver.start();
}

void ServerClient::join() {
    // Cierro los threads
    sender.join();
    receiver.join();
    is_alive = false;
}

bool ServerClient::is_dead() { return not is_alive; }

void ServerClient::kill() {
    sender_q.close();
    sk.shutdown(2);
    sk.close();
}

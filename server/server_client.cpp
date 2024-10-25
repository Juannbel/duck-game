#include "server_client.h"

#include <cstdio>
#include <utility>

#include <netinet/in.h>

#include "common/blocking_queue.h"
#include "common/map_dto.h"

#define SIZE_QUEUE 0

ServerClient::ServerClient(Socket&& sk, Queue<struct action>& gameloop_q, int id,
                           MatchInfo match_info):
        sk(std::move(sk)),
        sender_q(Queue<Snapshot>(SIZE_QUEUE)),
        protocolo(ServerProtocol(this->sk)),
        sender(protocolo, sender_q, match_info.duck_id),
        receiver(protocolo, gameloop_q, match_info.duck_id),
        id(id) {
    protocolo.send_match_info(match_info);
}

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

#include "server_client.h"

#include <cstdio>
#include <utility>

#include <netinet/in.h>

#include "common/blocking_queue.h"

#define SIZE_QUEUE 0

ServerClient::ServerClient(Socket&& sk):
        sk(std::move(sk)),
        sender_q(Queue<Snapshot>(SIZE_QUEUE)),
        protocolo(ServerProtocol(this->sk)),
        sender(protocolo, sender_q),
        receiver(protocolo) {}

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

int ServerClient::receive_cmd() {
    return protocolo.receive_cmd();
}

// TODO: cambiar esto, no me gusta (setters)
void ServerClient::set_gameloop_queue(Queue<struct action>* queue) {
    receiver.set_gameloop_queue(queue);
}

// TODO: cambiar esto, no me gusta (setters)
void ServerClient::send_game_info(MatchInfo match_info) {
    protocolo.send_match_info(match_info);
    receiver.set_duck_id(match_info.duck_id);
    sender.set_duck_id(match_info.duck_id);
}

void ServerClient::send_games_info(std::vector<int> list_lobbies) {
    for (int lobby: list_lobbies) {
        protocolo.send_lobby_info(lobby);
    }
}

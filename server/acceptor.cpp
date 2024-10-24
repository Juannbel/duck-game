#include "acceptor.h"

#include <cstdio>
#include <exception>
#include <utility>

#include "common/blocking_queue.h"
#include "common/map_dto.h"

#include "server_client.h"

Acceptor::Acceptor(Socket& sk, Queue<struct action>& gameloop_q, QueueListMonitor& sv_msg_queues, Map& map):
        sk(sk), gameloop_q(gameloop_q), sv_msg_queues(sv_msg_queues), map(map) {}

void Acceptor::run() {
    int id = 0;  // Estaria bueno usar un uuid
    while (true) {
        try {
            Socket peer = sk.accept();

            // por ahora mandamos el id del cliente como id del jugador
            ServerClient* th = new ServerClient(std::move(peer), gameloop_q, id, MatchInfo(id%4, map));
            sv_msg_queues.add_element(&(th->get_sender_queue()));

            th->start();

            reap_dead();
            clients.push_back(th);
            id++;
        } catch (const std::exception& e) {
            // sk.accept falla (se cerro el socket) por lo que corto el while
            break;
        }
    }
    kill_all();
}

void Acceptor::reap_dead() {
    clients.remove_if([this](ServerClient* c) {
        if (c->is_dead()) {
            // Sacar la sender_q de la protected list
            sv_msg_queues.remove_element(&(c->get_sender_queue()));  // Para usarla asi tengo q usar
                                                                     // mi implementacion de monitor
            c->join();
            return true;
        }
        return false;
    });
}

void Acceptor::kill_all() {
    for (auto& c: clients) {
        c->kill();
        c->join();
        delete c;
    }
    clients.clear();
}

int Acceptor::get_clients_count() {
    return clients.size();
}

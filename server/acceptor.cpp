#include "acceptor.h"

#include <cstdio>
#include <exception>
#include <utility>

#include "common/blocking_queue.h"

#include "ServerClient.h"

Acceptor::Acceptor(Socket& sk, Queue<struct action>& gameloop_q, QueueListMonitor& sv_msg_queues):
        sk(sk), gameloop_q(gameloop_q), sv_msg_queues(sv_msg_queues) {}

void Acceptor::run() {
    int id = 0;  // Estaria bueno usar un uuid
    while (true) {
        try {
            Socket peer = sk.accept();

            ServerClient* th = new ServerClient(std::move(peer), gameloop_q, id);
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

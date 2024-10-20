#include <cstdio>
#include <exception>

#include "libs/queue.h"

#include "aceptador.h"
#include "server_client.h"

Aceptador::Aceptador(Socket& sk, Queue<Snapshot>& gameloop_q, QueueListMonitor& sv_msg_queues):
        sk(sk), gameloop_q(gameloop_q), sv_msg_queues(sv_msg_queues) {}

void Aceptador::run() {
    int id = 0;  // Estaria bueno usar un uuid
    while (true) {
        try {
            Socket peer = sk.accept();

            ServerClient* th = new ServerClient(std::move(peer), gameloop_q, id);
            sv_msg_queues.add(th->get_sender_queue(), id);

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

void Aceptador::reap_dead() {
    clients.remove_if([this](ServerClient* c) {
        if (c->is_dead()) {
            // Sacar la sender_q de la protected list
            sv_msg_queues.remove(c->get_id()); // Para usarla asi tengo q usar mi implementacion de monitor
            c->join();
            return true;
        }
        return false;
    });
}

void Aceptador::kill_all() {
    for (auto& c: clients) {
        c->kill();
        c->join();
        delete c;
    }
    clients.clear();
}

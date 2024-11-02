#include "acceptor.h"

#include <cstdio>
#include <exception>
#include <utility>

#include "games_monitor.h"
#include "server_client.h"

Acceptor::Acceptor(Socket& sk): sk(sk) {}

void Acceptor::run() {
    int id = 0;
    GamesMonitor games_monitor;
    while (true) {
        try {
            Socket peer = sk.accept();

            ServerClient* th = new ServerClient(std::move(peer), games_monitor, id);
            id++;

            th->start();

            reap_dead();
            players.push_back(th);
        } catch (const std::exception& e) {
            // sk.accept falla (se cerro el socket) por lo que corto el while
            break;
        }
    }
    kill_all();
}

void Acceptor::reap_dead() {
    players.remove_if([](ServerClient* c) {
        if (c->is_dead()) {
            c->join();
            return true;
        }
        return false;
    });
}

void Acceptor::kill_all() {
    for (auto& c: players) {
        c->kill();
        c->join();
        delete c;
    }
    players.clear();
}

int Acceptor::get_clients_count() { return players.size(); }

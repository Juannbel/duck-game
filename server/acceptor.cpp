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
    games_monitor.delete_games();
    kill_all();
}

void Acceptor::reap_dead() {
    players.remove_if([](ServerClient* player) {
        if (player->is_dead()) {
            player->join();
            delete player;
            return true;
        }
        return false;
    });
}

void Acceptor::kill_all() {
    for (auto& player: players) {
        player->kill();
        player->join();
        delete player;
    }
    players.clear();
}

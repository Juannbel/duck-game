#include "server.h"

#include <cstdio>
#include "server/game_loop.h"
#include "server/maps/yaml.h"

#define EXIT 'q'
#define EXPECTED_PLAYERS 2

Server::Server(const char* port):
        sk(Socket(port)),
        sv_msg_queues(QueueListMonitor()),
        map(map_loader.loadMap(SERVER_DATA_PATH "/map1.yaml")),
        thAcceptor(sk, actions_q, sv_msg_queues, map) {}

void Server::start() {
    thAcceptor.start();

    while (thAcceptor.get_clients_count() != EXPECTED_PLAYERS) {
        sleep(1);
    }

    std::cout << "All players connected, starting gameloop" << std::endl;
    GameLoop gameloop(actions_q, sv_msg_queues, map, EXPECTED_PLAYERS);
    gameloop.start();

    char c = getchar();
    while (c != EXIT) {
        c = getchar();
    }

    gameloop.stop();
    gameloop.join();
}

Server::~Server() {
    sk.shutdown(2);
    sk.close();

    thAcceptor.join();
}

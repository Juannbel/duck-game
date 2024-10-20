#include "server.h"

#include <cstdio>

#define EXIT 'q'

Server::Server(const char* puerto):
        sk(Socket(puerto)),
        sv_msg_queues(QueueListMonitor()),
        gameloop(sv_msg_queues),
        thAceptador(sk, gameloop.getQueue(), sv_msg_queues) {}

void Server::start() {
    thAceptador.start();
    gameloop.start();

    char c = getchar();
    while (c != EXIT) {
        c = getchar();
    }
}

Server::~Server() {
    sk.shutdown(2);
    sk.close();

    thAceptador.join();
    gameloop.kill();
    gameloop.join();
}

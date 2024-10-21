#include "server.h"

#include <cstdio>

#define EXIT 'q'

Server::Server(const char* puerto):
        sk(Socket(puerto)),
        sv_msg_queues(QueueListMonitor()),
        gameloop(sv_msg_queues),
        thAcceptor(sk, gameloop.getQueue(), sv_msg_queues) {}

void Server::start() {
    thAcceptor.start();
    gameloop.start();

    char c = getchar();
    while (c != EXIT) {
        c = getchar();
    }
}

Server::~Server() {
    sk.shutdown(2);
    sk.close();

    thAcceptor.join();
    gameloop.kill();
    gameloop.join();
}

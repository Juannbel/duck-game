#include "server.h"

#include <cstdio>

#define EXIT 'q'

Server::Server(const char* port):
        sk(Socket(port)),
        sv_msg_queues(QueueListMonitor()),
        actions_q(),
        gameloop(actions_q, sv_msg_queues, 1),
        thAcceptor(sk, actions_q, sv_msg_queues) {}

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
    gameloop.stop();
    gameloop.join();
}

#include "server.h"

#include <cstdio>

#define EXIT 'q'

Server::Server(const char* port): sk(Socket(port)), thAcceptor(sk) {}

void Server::start() {
    thAcceptor.start();

    char c = getchar();
    while (c != EXIT) {
        c = getchar();
    }
}

Server::~Server() {
    sk.shutdown(2);
    sk.close();

    thAcceptor.join();
}

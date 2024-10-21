#include <cstdio>

#include "server.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        perror("argumentos invalidos");
        return 1;
    }

    // Inicializo el server y la comunicación con el cliente
    Server server(argv[1]);

    server.start();

    return 0;
}

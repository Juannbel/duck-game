#include <cstdio>
#include <iostream>

#include "server.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        perror("argumentos invalidos");
        return 1;
    }

    // Inicializo el server y la comunicación con el cliente
    try {
        Server server(argv[1]);
        server.start();
    } catch (const std::exception& e) {
        std::cerr << "Something went wrong: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Something went wrong" << std::endl;
        return 1;
    }

    return 0;
}

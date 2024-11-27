#include <cstdio>
#include <iostream>

#include "server.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Argumentos invalidos usar: " << argv[0] << " <puerto>" << std::endl;
        return 1;
    }

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

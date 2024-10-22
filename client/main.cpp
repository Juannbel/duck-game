#include <cstdlib>
#include <iostream>

#include "client.h"

#define ARG_HOSTNAME 1
#define ARG_SERVNAME 2

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Bad call. Usage: " << argv[0] << " <hostname> <servname>" << std::endl;
        return EXIT_FAILURE;
    }

    try {
        Client client(argv[ARG_HOSTNAME], argv[ARG_SERVNAME]);
        client.run();
        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        std::cerr << "Something went wrong: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Something went wrong" << std::endl;
        return EXIT_FAILURE;
    }
}

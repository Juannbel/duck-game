#include "client.h"
#include "common/map_dto.h"

#include "constant_looper.h"

Client::Client(const char* hostname, const char* servname):
        protocol(Socket(hostname, servname)),
        receiver(protocol, snapshot_q),
        sender(protocol, command_q) {}

void Client::run() {
    MatchInfo match_info = protocol.recv_match_info();
    std::cout << "Match info received" << std::endl;
    std::cout << "Duck id: " << (int) match_info.duck_id << std::endl;

    receiver.start();
    sender.start();

    ConstantLooper looper(match_info, snapshot_q, command_q);
    looper.run();

    std::cout << "Game ended" << std::endl;
}

Client::~Client() {
    protocol.shutdown();
    snapshot_q.close();
    command_q.close();
    receiver.join();
    sender.join();
}

#ifndef CLIENT_H
#define CLIENT_H

#include "client_protocol.h"
#include "client_receiver.h"
#include "client_sender.h"

class Client {
private:
    ClientProtocol protocol;
    Queue<Command> command_q;
    Queue<Snapshot> snapshot_q;
    ClientReceiver receiver;
    ClientSender sender;

public:
    Client(const char* hostname, const char* servname);

    void run();

    int displayMenuAndGetOption();

    void joinLobby();

    void displayLobbyList();

    int getLobbyIdFromUser();

    ~Client();
};

#endif

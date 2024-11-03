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

    int32_t display_menu_and_get_option();

    bool joinLobby(uint8_t &duck_id);

    void display_lobbies();

    int32_t get_lobby_id();

    ~Client();
};

#endif

#include <QApplication>
#include <iostream>
#include <utility>

#include "client/client_protocol.h"
#include "common/socket.h"

#include "mainwindow.h"

class Lobby {
private:
    int argc = 1;
    char* argv[2] = {strdup("Duck game lobby"), nullptr};
    QApplication app;
    MainWindow mainWindow;

public:
    Lobby(ClientProtocol& protocol, std::pair<uint8_t, uint8_t>& duck_ids, bool& ready_to_play):
            app(argc, argv), mainWindow(nullptr, protocol, duck_ids, ready_to_play) {}

    void run() {
        mainWindow.show();
        app.exec();
    }

    ~Lobby() { free(argv[0]); }
};

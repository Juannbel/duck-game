#include <QApplication>

#include "../client_protocol.h"

#include "mainwindow.h"

class Lobby {
private:
    int argc = 1;
    char* argv[2] = {strdup("Duck game lobby"), nullptr};
    QApplication app;
    MainWindow mainWindow;

public:
    Lobby(ClientProtocol& protocol, uint8_t& duck_id):
            app(argc, argv), mainWindow(nullptr, protocol, duck_id) {}

    void run() {
        mainWindow.show();
        app.exec();
    }

    ~Lobby() { free(argv[0]); }
};
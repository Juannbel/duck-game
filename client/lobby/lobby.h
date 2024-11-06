#include <QApplication>
#include <array>

#include "../client_protocol.h"

#include "mainwindow.h"

class Lobby {
private:
    int argc = 1;
    char* argv[2] = {strdup("Duck game lobby"), nullptr};
    QApplication app;
    MainWindow mainWindow;

public:
    Lobby(ClientProtocol& protocol, std::array<uint8_t, 2>& duck_ids):
            app(argc, argv), mainWindow(nullptr, protocol, duck_ids) {}

    void run() {
        mainWindow.show();
        app.exec();
    }

    ~Lobby() { free(argv[0]); }
};
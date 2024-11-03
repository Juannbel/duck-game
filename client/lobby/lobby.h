#include <QApplication>

#include "mainwindow.h"
#include "../client_protocol.h"

class Lobby {
private:
    QApplication app;
    MainWindow mainWindow;

public:
    Lobby(int argc, char* argv[], ClientProtocol &protocol, uint8_t &duck_id):
        app(argc, argv),
        mainWindow(nullptr, protocol, duck_id) {}

    void run() {
        mainWindow.show();
        app.exec();
    }
};
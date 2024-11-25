#ifndef PRE_LOBBY_H
#define PRE_LOBBY_H

#include <QApplication>
#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <qapplication.h>

#include "client/client_protocol.h"
#include "common/socket.h"

#include "prelobbymainwindow.h"

class PreLobby {
private:
    std::string hostname;
    std::string servname;
    QApplication& app;
    PreLobbyMainWindow mainWindow;
    std::optional<Socket> socket;

public:
    PreLobby(QApplication& app, bool& connected):
            app(app), mainWindow(nullptr, socket, hostname, servname, connected) {}

    void run() {
        mainWindow.show();
        app.exec();
    }

    Socket get_socket() {
        // Para la primera partida va a tener el valor dado en mainWindow por haber probado la conexión, despues lo dejamos en nullopt
        if (socket.has_value()) {
            auto ret = std::move(socket.value());
            socket = std::nullopt;
            return ret;
        }

        return Socket(hostname.c_str(), servname.c_str());
    }

    ~PreLobby() {}
};

#endif

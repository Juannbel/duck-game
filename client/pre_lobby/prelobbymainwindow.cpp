#include "prelobbymainwindow.h"
#include <iostream>

#include "./ui_prelobbymainwindow.h"

PreLobbyMainWindow::PreLobbyMainWindow(QWidget* parent, std::optional<Socket>& socket, std::string& hostname,
                    std::string& servname, bool& connected) :
        QMainWindow(parent), ui(new Ui::PreLobbyMainWindow), socket(socket), hostname(hostname), servname(servname), connected(connected) {
    ui->setupUi(this);
    
    connect(ui->connect_to_server_button, &QPushButton::clicked, this, &PreLobbyMainWindow::onConnectClicked);
}

void PreLobbyMainWindow::onConnectClicked() {
    hostname = ui->hostname_line->text().toStdString();
    servname = ui->servname_line->text().toStdString();

    try {
        socket = Socket(hostname.c_str(), servname.c_str());
        connected = true;
        close();
    } catch (...) {
        std::cerr << "ERROR" << std::endl;
    }

}

PreLobbyMainWindow::~PreLobbyMainWindow() { delete ui; }

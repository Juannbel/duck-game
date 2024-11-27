#include "prelobbymainwindow.h"

#include <QFontDatabase>
#include <QMessageBox>
#include <string>

#include <qapplication.h>
#include <qlineedit.h>
#include <qpainter.h>

#include "./ui_prelobbymainwindow.h"

PreLobbyMainWindow::PreLobbyMainWindow(QWidget* parent, std::optional<Socket>& socket,
                                       std::string& hostname, std::string& servname,
                                       bool& connected):
        QMainWindow(parent),
        ui(new Ui::PreLobbyMainWindow),
        socket(socket),
        hostname(hostname),
        servname(servname),
        connected(connected) {
    ui->setupUi(this);

    connect(ui->connect_to_server_button, &QPushButton::clicked, this,
            &PreLobbyMainWindow::onConnectClicked);

    QPixmap logoPixmap(DATA_PATH "/logo.png");
    ui->logo->setPixmap(logoPixmap);

    QPixmap bkgnd(DATA_PATH "/backgrounds/lobby.png");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPainter painter(&bkgnd);
    painter.fillRect(bkgnd.rect(), QColor(0, 0, 0, 150));
    painter.end();
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    this->setPalette(palette);

    int id = QFontDatabase::addApplicationFont(DATA_PATH "/fonts/primary.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont font(family);
    this->setFont(font);
}

void PreLobbyMainWindow::onConnectClicked() {
    hostname = ui->hostname_line->text().toStdString();
    servname = ui->servname_line->text().toStdString();

    try {
        socket = Socket(hostname.c_str(), servname.c_str());
        connected = true;
        close();
    } catch (...) {
        QMessageBox::critical(this, "Error connecting to server",
                              "Please check the hostname and servname and try again.");
    }
}

PreLobbyMainWindow::~PreLobbyMainWindow() { delete ui; }

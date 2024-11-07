#include "mainwindow.h"

#include <QMessageBox>
#include <iostream>
#include <vector>
#include <qinputdialog.h>
#include <qresource.h>
#include <QFile>
#include <QResource>

#include "../../common/lobby.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent, ClientProtocol& protocol, std::pair<uint8_t, uint8_t>& duck_ids):
        QMainWindow(parent), ui(new Ui::MainWindow), protocol(protocol), duck_ids(duck_ids) {
    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex(0);

    connect(ui->createGameButton, &QPushButton::clicked, this, &MainWindow::onCreateGameClicked);
    connect(ui->joinGameButton, &QPushButton::clicked, this, &MainWindow::onJoinGameClicked);

    connect(ui->backButton, &QPushButton::clicked, this, &MainWindow::onBackClicked);
    connect(ui->joinLobbyButton, &QPushButton::clicked, this, &MainWindow::onJoinLobbyClicked);

    connect(ui->startGameButton, &QPushButton::clicked, this, &MainWindow::onStartGameClicked);

    QPixmap logoPixmap(DATA_PATH "/logo.png");

    ui->p3logo->setPixmap(logoPixmap);
    ui->p1logo->setPixmap(logoPixmap);
    // centramos horizontalmente
    ui->p3logo->setAlignment(Qt::AlignCenter);
    ui->p1logo->setAlignment(Qt::AlignCenter);
}

void MainWindow::onCreateGameClicked() {
    bool ok;
    int numPlayers = QInputDialog::getInt(this, tr("How many players?"),
                                          tr("Number of players (1 o 2):"), 1, 1, 2, 1, &ok);
    if (!ok) return;

    protocol.send_option(CREATE_GAME);
    protocol.send_option(numPlayers);

    GameInfo gameInfo = protocol.recv_game_info();
    duck_ids.first = gameInfo.duck_id_1;
    duck_ids.second = gameInfo.duck_id_2;

    ui->gameIdLabel->setText(QString("Game ID: %1").arg(gameInfo.game_id));
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::onJoinGameClicked() {
    protocol.send_option(LIST_GAMES);
    updateLobbyList();
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::onBackClicked() { ui->stackedWidget->setCurrentIndex(0); }

void MainWindow::onJoinLobbyClicked() {
    int selectedRow = ui->lobbiesList->currentRow();
    if (selectedRow < 0) {
        QMessageBox::warning(this, "Error", "Select a lobby to join");
        return;
    }

    int32_t lobbyId = ui->lobbiesList->item(selectedRow)->text().toInt();

    bool ok;
    int numPlayers = QInputDialog::getInt(this, tr("How many players?"),
                                          tr("Number of players (1 o 2):"), 1, 1, 2, 1, &ok);
    if (!ok) return;

    protocol.send_option(JOIN_GAME);
    protocol.send_option(lobbyId);
    protocol.send_option(numPlayers);

    GameInfo gameInfo = protocol.recv_game_info();
    if (gameInfo.game_id != INVALID_GAME_ID) {
        duck_ids.first = gameInfo.duck_id_1;
        duck_ids.second = gameInfo.duck_id_2;
        close();
    } else {
        QMessageBox::warning(this, "Error", "Could not join the game.");
    }
}

void MainWindow::onStartGameClicked() {
    protocol.send_option(0);
    close();
}

void MainWindow::updateLobbyList() {
    ui->lobbiesList->clear();
    std::vector<int32_t> lobbies = protocol.recv_lobbies_info();

    for (int32_t lobby: lobbies) {
        ui->lobbiesList->addItem(QString::number(lobby));
    }
}

MainWindow::~MainWindow() { delete ui; }

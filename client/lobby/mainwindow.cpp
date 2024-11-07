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

    connect(ui->namesConfirmButton, &QPushButton::clicked, this, &MainWindow::onCreateGameConfirmed);
    connect(ui->namesBackButton, &QPushButton::clicked, this, &MainWindow::onBackClicked);
    connect(ui->singlePlayerRadio, &QRadioButton::toggled,
            [this](bool checked) { ui->player2NameEdit->setEnabled(!checked); });
    QPixmap logoPixmap(DATA_PATH "/logo.png");

    ui->p3logo->setPixmap(logoPixmap);
    ui->p1logo->setPixmap(logoPixmap);
    // centramos horizontalmente
    ui->p3logo->setAlignment(Qt::AlignCenter);
    ui->p1logo->setAlignment(Qt::AlignCenter);
}

void MainWindow::onCreateGameClicked() {
    disconnect(ui->namesConfirmButton, nullptr, this, nullptr);
    connect(ui->namesConfirmButton, &QPushButton::clicked, this, &MainWindow::onCreateGameConfirmed);
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::onCreateGameConfirmed() {
    QString player1Name = ui->player1NameEdit->text().trimmed();
    QString player2Name = ui->player2NameEdit->text().trimmed();

    if (player1Name.isEmpty()) {
        QMessageBox::warning(this, "Error", "Player 1 name is required");
        return;
    }

    if (ui->multiPlayerRadio->isChecked() && player2Name.isEmpty()) {
        QMessageBox::warning(this, "Error", "Player 2 name is required");
        return;
    }

    int numPlayers = ui->multiPlayerRadio->isChecked() ? 2 : 1;

    protocol.send_option(CREATE_GAME);
    protocol.send_option(numPlayers);
    protocol.send_string(player1Name.toStdString());
    if (numPlayers == 2) {
        protocol.send_string(player2Name.toStdString());
    }

    GameInfo gameInfo = protocol.recv_game_info();
    duck_ids.first = gameInfo.duck_id_1;
    duck_ids.second = gameInfo.duck_id_2;
    ui->gameIdLabel->setText(QString("Game ID: %1").arg(gameInfo.game_id));
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::onJoinGameConfirmed() {
    if (selected_lobby_row < 0) {
        QMessageBox::warning(this, "Error", "Select a lobby to join");
        return;
    }

    QString player1Name = ui->player1NameEdit->text().trimmed();
    QString player2Name = ui->player2NameEdit->text().trimmed();

    if (player1Name.isEmpty()) {
        QMessageBox::warning(this, "Error", "Player 1 name is required");
        return;
    }

    if (ui->multiPlayerRadio->isChecked() && player2Name.isEmpty()) {
        QMessageBox::warning(this, "Error", "Player 2 name is required");
        return;
    }

    int numPlayers = ui->multiPlayerRadio->isChecked() ? 2 : 1;

    int32_t game_id = ui->lobbiesList->item(selected_lobby_row)->text().toInt();
    protocol.send_option(JOIN_GAME);
    protocol.send_option(game_id);
    protocol.send_option(numPlayers);
    protocol.send_string(player1Name.toStdString());
    if (numPlayers == 2) {
       protocol.send_string(player2Name.toStdString());
    }

    GameInfo gameInfo = protocol.recv_game_info();
    if (gameInfo.game_id != INVALID_GAME_ID) {
        duck_ids.first = gameInfo.duck_id_1;
        duck_ids.second = gameInfo.duck_id_2;
        close();
    } else {
        QMessageBox::warning(this, "Error", "Could not join the game.");
    }
}

void MainWindow::onJoinGameClicked() {
    protocol.send_option(LIST_GAMES);
    updateLobbyList();
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::onBackClicked() { ui->stackedWidget->setCurrentIndex(0); }

void MainWindow::onJoinLobbyClicked() {
    selected_lobby_row = ui->lobbiesList->currentRow();
    if (selected_lobby_row < 0) {
        QMessageBox::warning(this, "Error", "Select a lobby to join");
        return;
    }

    disconnect(ui->namesConfirmButton, nullptr, this, nullptr);
    connect(ui->namesConfirmButton, &QPushButton::clicked, this, &MainWindow::onJoinGameConfirmed);
    ui->stackedWidget->setCurrentIndex(3);
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

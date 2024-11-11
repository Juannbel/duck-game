#include "mainwindow.h"

#include <QFile>
#include <QMessageBox>
#include <QResource>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <iostream>
#include <utility>
#include <vector>

#include <qinputdialog.h>
#include <qpushbutton.h>
#include <qresource.h>

#include "../../common/lobby.h"
#include "./ui_mainwindow.h"
#include "common/snapshot.h"

MainWindow::MainWindow(QWidget* parent, ClientProtocol& protocol,
                       std::pair<uint8_t, uint8_t>& duck_ids, bool& ready_to_play):
        QMainWindow(parent), ui(new Ui::MainWindow), protocol(protocol), duck_ids(duck_ids), ready_to_play(ready_to_play) {
    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex(0);

    connect(ui->createGameButton, &QPushButton::clicked, this, &MainWindow::onCreateGameClicked);
    connect(ui->joinGameButton, &QPushButton::clicked, this, &MainWindow::onJoinGameClicked);

    connect(ui->backButton, &QPushButton::clicked, this, &MainWindow::onBackClicked);
    connect(ui->joinLobbyButton, &QPushButton::clicked, this, &MainWindow::onJoinLobbyClicked);
    connect(ui->refreshLobbiesButton, &QPushButton::clicked, this, &MainWindow::onRefreshLobbiesClicked);

    connect(ui->startGameButton, &QPushButton::clicked, this, &MainWindow::onStartGameClicked);
    connect(ui->refreshConnectedButton, &QPushButton::clicked, this, &MainWindow::onRefreshConnectedClicked);

    connect(ui->namesConfirmButton, &QPushButton::clicked, this,
            &MainWindow::onCreateGameConfirmed);
    connect(ui->namesBackButton, &QPushButton::clicked, this, &MainWindow::onBackClicked);

    ui->player2NameEdit->setVisible(false);
    connect(ui->singlePlayerRadio, &QRadioButton::toggled, [this](bool checked) {
        ui->player2NameEdit->setEnabled(!checked);
        ui->player2NameEdit->setVisible(!checked);
    });


    QPixmap logoPixmap(DATA_PATH "/logo.png");

    ui->p3logo->setPixmap(logoPixmap);
    ui->p1logo->setPixmap(logoPixmap);
    ui->p3logo->setAlignment(Qt::AlignCenter);
    ui->p1logo->setAlignment(Qt::AlignCenter);
}

void MainWindow::onCreateGameClicked() {
    disconnect(ui->namesConfirmButton, nullptr, this, nullptr);
    connect(ui->namesConfirmButton, &QPushButton::clicked, this,
            &MainWindow::onCreateGameConfirmed);
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
    game_id = gameInfo.game_id;
    onRefreshConnectedClicked();
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

    int32_t game_id = lobbies_info[selected_lobby_row].game_id;
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
        game_id = gameInfo.game_id;
        ready_to_play = true;
        close();
    } else {
        QMessageBox::warning(this, "Error", "Could not join the game.");
    }
}

void MainWindow::onJoinGameClicked() {
    onRefreshLobbiesClicked();
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
    protocol.send_option(START_GAME);
    ready_to_play = true;
    close();
}

void MainWindow::onRefreshConnectedClicked() {
    protocol.send_option(GET_INFO);
    std::vector<LobbyInfo> lobbies_info = protocol.recv_lobbies_info();
    if (lobbies_info[0].game_id != game_id) {
        return;
    }
    ui->gameIdLabel->setText(QString("Game ID: %1 \nConnected players: %2/%3").arg(game_id).arg(lobbies_info[0].connected_players).arg(MAX_DUCKS));
}

void MainWindow::onRefreshLobbiesClicked() {
    protocol.send_option(LIST_GAMES);
    ui->lobbiesList->clear();
    lobbies_info = protocol.recv_lobbies_info();

    ui->lobbiesList->setColumnCount(3);
    ui->lobbiesList->setRowCount(lobbies_info.size());

    QStringList headers;
    headers << "Game ID"
            << "Players"
            << "Creator";
    ui->lobbiesList->setHorizontalHeaderLabels(headers);

    ui->lobbiesList->verticalHeader()->setVisible(false);

    int row = 0;
    for (const LobbyInfo& lobby: lobbies_info) {
        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(lobby.game_id));
        idItem->setTextAlignment(Qt::AlignCenter);
        ui->lobbiesList->setItem(row, 0, idItem);

        QString playersText = QString("%1/%2").arg(lobby.connected_players).arg(MAX_DUCKS);
        QTableWidgetItem* playersItem = new QTableWidgetItem(playersText);
        playersItem->setTextAlignment(Qt::AlignCenter);
        ui->lobbiesList->setItem(row, 1, playersItem);

        QTableWidgetItem* creatorItem = new QTableWidgetItem(lobby.creator);
        creatorItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->lobbiesList->setItem(row, 2, creatorItem);

        row++;
    }

    ui->lobbiesList->setAlternatingRowColors(true);
    ui->lobbiesList->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->lobbiesList->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->lobbiesList->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

MainWindow::~MainWindow() { delete ui; }

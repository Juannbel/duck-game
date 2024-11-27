#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QFontDatabase>
#include <QMessageBox>
#include <QResource>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <utility>
#include <vector>

#include <qinputdialog.h>
#include <qpainter.h>
#include <qpushbutton.h>
#include <qresource.h>

#include "../../common/lobby.h"
#include "./ui_mainwindow.h"
#include "common/snapshot.h"

MainWindow::MainWindow(QWidget* parent, ClientProtocol& protocol,
                       std::pair<uint8_t, uint8_t>& duck_ids, bool& ready_to_play):
        QMainWindow(parent),
        ui(new Ui::MainWindow),
        protocol(protocol),
        duck_ids(duck_ids),
        ready_to_play(ready_to_play) {
    ui->setupUi(this);

    int id = QFontDatabase::addApplicationFont(DATA_PATH "/fonts/primary.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont font(family);
    font.setPointSize(18);

    this->setFont(font);

    ui->stackedWidget->setCurrentIndex(0);

    connect(ui->createGameButton, &QPushButton::clicked, this, &MainWindow::onCreateGameClicked);
    connect(ui->joinGameButton, &QPushButton::clicked, this, &MainWindow::onJoinGameClicked);

    connect(ui->backButton, &QPushButton::clicked, this, &MainWindow::onBackClicked);
    connect(ui->joinLobbyButton, &QPushButton::clicked, this, &MainWindow::onJoinLobbyClicked);
    connect(ui->refreshLobbiesButton, &QPushButton::clicked, this,
            &MainWindow::onRefreshLobbiesClicked);

    connect(ui->startGameButton, &QPushButton::clicked, this, &MainWindow::onStartGameClicked);
    connect(ui->refreshConnectedButton, &QPushButton::clicked, this,
            &MainWindow::onRefreshConnectedClicked);

    connect(ui->namesConfirmButton, &QPushButton::clicked, this,
            &MainWindow::onCreateGameConfirmed);
    connect(ui->namesBackButton, &QPushButton::clicked, this, &MainWindow::onBackClicked);

    ui->player2NameEdit->setVisible(false);
    QPixmap single_player(DATA_PATH "/others/single_player.png");
    QPixmap two_players(DATA_PATH "/others/two_players.png");

    single_player = single_player.scaled(ui->duck->size(), Qt::KeepAspectRatio);
    two_players = two_players.scaled(ui->duck->size(), Qt::KeepAspectRatio);
    ui->duck->setPixmap(single_player);
    ui->duck->setAlignment(Qt::AlignCenter);


    connect(ui->singlePlayerRadio, &QRadioButton::toggled,
            [single_player, two_players, this](bool checked) {
                ui->player2NameEdit->setEnabled(!checked);
                ui->player2NameEdit->setVisible(!checked);
                const QPixmap& pixmap = checked ? single_player : two_players;
                ui->duck->setPixmap(pixmap);
            });

    QPixmap logoPixmap(DATA_PATH "/logo.png");

    ui->p3logo->setPixmap(logoPixmap);
    ui->p1logo->setPixmap(logoPixmap);
    ui->p3logo->setAlignment(Qt::AlignCenter);
    ui->p1logo->setAlignment(Qt::AlignCenter);

    QPixmap bkgnd(DATA_PATH "/backgrounds/lobby.png");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPainter painter(&bkgnd);
    painter.fillRect(bkgnd.rect(), QColor(0, 0, 0, 150));
    painter.end();
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    this->setPalette(palette);
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

    GameInfo gameInfo;
    bool ok = executeSafely([this, &numPlayers, &player1Name, &player2Name, &gameInfo]() {
        protocol.send_option(CREATE_GAME);
        protocol.send_option(numPlayers);
        protocol.send_string(player1Name.toStdString());
        if (numPlayers == 2) {
            protocol.send_string(player2Name.toStdString());
        }

        gameInfo = protocol.recv_game_info();
    });

    if (!ok) {
        return;
    }

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

    GameInfo gameInfo;
    bool ok = executeSafely([this, &numPlayers, &player1Name, &player2Name, &game_id, &gameInfo]() {
        protocol.send_option(JOIN_GAME);
        protocol.send_option(game_id);
        protocol.send_option(numPlayers);
        protocol.send_string(player1Name.toStdString());
        if (numPlayers == 2) {
            protocol.send_string(player2Name.toStdString());
        }

        gameInfo = protocol.recv_game_info();
    });

    if (!ok) {
        return;
    }

    if (gameInfo.game_id != INVALID_GAME_ID) {
        duck_ids.first = gameInfo.duck_id_1;
        duck_ids.second = gameInfo.duck_id_2;
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
    bool started = false;
    bool ok = executeSafely([this, &started]() {
        protocol.send_option(START_GAME);
        if (protocol.recv_option() != CREATE_OK) {
            QMessageBox::warning(this, "Error", "Not enough players to start the game");
            return;
        }
        started = true;
    });

    if (!ok || !started) {
        return;
    }

    ready_to_play = true;
    close();
}

void MainWindow::onRefreshConnectedClicked() {
    std::vector<LobbyInfo> lobbies_info;
    bool ok = executeSafely([this, &lobbies_info]() {
        protocol.send_option(GET_INFO);
        lobbies_info = protocol.recv_lobbies_info();
    });

    if (!ok) {
        return;
    }

    if (lobbies_info[0].game_id != game_id) {
        return;
    }
    ui->gameIdLabel->setText(QString("ID: %1 \nConnected: %2 of %3")
                                     .arg(game_id)
                                     .arg(lobbies_info[0].connected_players)
                                     .arg(MAX_DUCKS));
}

void MainWindow::onRefreshLobbiesClicked() {
    ui->lobbiesList->clear();
    bool ok = executeSafely([this]() {
        protocol.send_option(LIST_GAMES);
        lobbies_info = protocol.recv_lobbies_info();
    });

    if (!ok) {
        return;
    }

    ui->lobbiesList->setColumnCount(3);
    ui->lobbiesList->setRowCount(lobbies_info.size());

    QStringList headers;
    headers << "ID"
            << "Connected"
            << "Creator";
    ui->lobbiesList->setHorizontalHeaderLabels(headers);

    ui->lobbiesList->verticalHeader()->setVisible(false);

    int row = 0;
    for (const LobbyInfo& lobby: lobbies_info) {
        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(lobby.game_id));
        idItem->setTextAlignment(Qt::AlignCenter);
        ui->lobbiesList->setItem(row, 0, idItem);

        QString playersText = QString("%1 of %2").arg(lobby.connected_players).arg(MAX_DUCKS);
        QTableWidgetItem* playersItem = new QTableWidgetItem(playersText);
        playersItem->setTextAlignment(Qt::AlignCenter);
        ui->lobbiesList->setItem(row, 1, playersItem);

        QTableWidgetItem* creatorItem = new QTableWidgetItem(lobby.creator);
        creatorItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->lobbiesList->setItem(row, 2, creatorItem);

        row++;
    }

    ui->lobbiesList->setColumnWidth(1, 160);

    ui->lobbiesList->setAlternatingRowColors(true);
    ui->lobbiesList->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->lobbiesList->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->lobbiesList->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

MainWindow::~MainWindow() { delete ui; }

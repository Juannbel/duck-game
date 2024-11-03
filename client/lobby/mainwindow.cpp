#include "mainwindow.h"

#include "./ui_mainwindow.h"
#include "../../common/lobby.h"
#include <QMessageBox>
#include <vector>

MainWindow::MainWindow(QWidget* parent, ClientProtocol& protocol, uint8_t& duck_id): QMainWindow(parent), ui(new Ui::MainWindow), protocol(protocol), duck_id(duck_id) {
    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex(0);

    connect(ui->createGameButton, &QPushButton::clicked, this, &MainWindow::onCreateGameClicked);
    connect(ui->joinGameButton, &QPushButton::clicked, this, &MainWindow::onJoinGameClicked);

    connect(ui->backButton, &QPushButton::clicked, this, &MainWindow::onBackClicked);
    connect(ui->joinLobbyButton, &QPushButton::clicked, this, &MainWindow::onJoinLobbyClicked);

    connect(ui->startGameButton, &QPushButton::clicked, this, &MainWindow::onStartGameClicked);
}

void MainWindow::onCreateGameClicked() {
    protocol.send_option(CREATE_GAME);
    GameInfo gameInfo = protocol.recv_game_info();
    duck_id = gameInfo.duck_id;

    ui->gameIdLabel->setText(QString("Game ID: %1").arg(gameInfo.game_id));
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::onJoinGameClicked() {
    protocol.send_option(LIST_GAMES);
    updateLobbyList();
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::onBackClicked() {
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::onJoinLobbyClicked() {
    int selectedRow = ui->lobbiesList->currentRow();
    if (selectedRow < 0) {
        QMessageBox::warning(this, "Error", "Selecciona un lobby para unirte.");
        return;
    }

    int32_t lobbyId = ui->lobbiesList->item(selectedRow)->text().toInt();
    protocol.send_option(JOIN_GAME);
    protocol.send_option(lobbyId);

    GameInfo gameInfo = protocol.recv_game_info();
    if (gameInfo.game_id != INVALID_GAME_ID) {
        duck_id = gameInfo.duck_id;
        close();
    } else {
        QMessageBox::warning(this, "Error", "No se pudo unir al lobby.");
    }
}

void MainWindow::onStartGameClicked() {
    protocol.send_option(0);
    close();
}

void MainWindow::updateLobbyList() {
    ui->lobbiesList->clear();
    std::vector<int32_t> lobbies = protocol.recv_lobbies_info();

    for (int32_t lobby : lobbies) {
        ui->lobbiesList->addItem(QString::number(lobby));
    }
}

MainWindow::~MainWindow() { delete ui; }

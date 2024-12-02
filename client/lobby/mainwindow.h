#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTimer>
#include <utility>
#include <vector>

#include <qdebug.h>

#include "../client_protocol.h"
#include "common/lobby.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow: public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent, ClientProtocol& protocol, std::pair<uint8_t, uint8_t>& duck_ids,
               bool& ready_to_play);
    ~MainWindow();

private slots:
    void onCreateGameClicked();
    void onJoinGameClicked();
    void onBackClicked();
    void onJoinLobbyClicked();
    void onStartGameClicked();
    void onCreateGameConfirmed();
    void onJoinGameConfirmed();
    void onRefreshConnectedClicked();
    void onRefreshLobbiesClicked();

private:
    Ui::MainWindow* ui;
    QTimer* timer;
    ClientProtocol& protocol;
    std::pair<uint8_t, uint8_t>& duck_ids;
    std::vector<LobbyInfo> lobbies_info;
    int selected_lobby_row = -1;
    int32_t game_id = -1;
    bool& ready_to_play;

    template <typename Func>
    bool executeSafely(Func&& func) {
        try {
            func();
        } catch (const SocketWasClosed& e) {
            QMessageBox::critical(this, "Error", "Server closed the connection");
            close();
            return false;
        }
        return true;
    }
};

#endif  // MAINWINDOW_H

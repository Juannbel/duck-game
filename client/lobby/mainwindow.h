#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "../client_protocol.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow: public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent, ClientProtocol& protocol, std::pair<uint8_t, uint8_t>& duck_ids);
    ~MainWindow();

private slots:
    void onCreateGameClicked();
    void onJoinGameClicked();
    void onBackClicked();
    void onJoinLobbyClicked();
    void onStartGameClicked();
    void onCreateGameConfirmed();
    void onJoinGameConfirmed();

private:
    Ui::MainWindow* ui;
    ClientProtocol& protocol;
    std::pair<uint8_t, uint8_t>& duck_ids;
    int selected_lobby_row = -1;

    void updateLobbyList();
};

#endif  // MAINWINDOW_H

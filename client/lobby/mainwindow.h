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
    MainWindow(QWidget* parent, ClientProtocol& protocol, uint8_t& duck_id);
    ~MainWindow();

private slots:
    void onCreateGameClicked();
    void onJoinGameClicked();
    void onBackClicked();
    void onJoinLobbyClicked();
    void onStartGameClicked();

private:
    Ui::MainWindow* ui;
    ClientProtocol& protocol;
    uint8_t& duck_id;

    void updateLobbyList();
};

#endif  // MAINWINDOW_H

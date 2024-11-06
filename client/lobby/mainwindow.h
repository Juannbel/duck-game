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
    MainWindow(QWidget* parent, ClientProtocol& protocol, std::array<uint8_t, 2>& duck_ids);
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
    std::array<uint8_t, 2>& duck_ids;

    void updateLobbyList();
};

#endif  // MAINWINDOW_H

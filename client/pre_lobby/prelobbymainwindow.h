#ifndef PRELOBBYMAINWINDOW_H
#define PRELOBBYMAINWINDOW_H

#include <QMainWindow>
#include <optional>
#include "common/socket.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class PreLobbyMainWindow;
}
QT_END_NAMESPACE

class PreLobbyMainWindow: public QMainWindow {
    Q_OBJECT

public:
    PreLobbyMainWindow(QWidget* parent, std::optional<Socket>& socket, std::string& hostname,
                       std::string& servname, bool& connected);
    ~PreLobbyMainWindow();

private slots:
    void onConnectClicked();

private:
    Ui::PreLobbyMainWindow* ui;
    std::optional<Socket>& socket;
    std::string& hostname;
    std::string& servname;
    bool& connected;
};
#endif  // PRELOBBYMAINWINDOW_H

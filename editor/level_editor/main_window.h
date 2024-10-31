#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "graphic_view_scenario.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    GraphicViewScenario *scenario;
};
#endif // MAIN_WINDOW_H

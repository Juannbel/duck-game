#include "main_window.h"
#include "./ui_mainwindow.h"
#include "./editor_constants.h"
#include "graphic_view_scenario.h"
#include <QSize>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
    scenario(new GraphicViewScenario())
{
    this->ui->setupUi(this);
    this->ui->graphicsView->setScene(this->scenario);

    QSize size(MAP_WIDTH_PIXELS, MAP_HEIGHT_PIXELS);
    this->scenario->setSceneRect(0, 0, size.width(), size.height());
    this->ui->spinBoxX->setMinimum(INT_MIN);
    this->ui->spinBoxX->setMaximum(INT_MAX);
    this->ui->spinBoxY->setMinimum(INT_MIN);
    this->ui->spinBoxY->setMaximum(INT_MAX);
    this->scenario->setSpinBox(this->ui->spinBoxX, this->ui->spinBoxY);
}

MainWindow::~MainWindow()
{
    delete scenario;
    delete ui;
}



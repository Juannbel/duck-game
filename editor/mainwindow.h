#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QVector>
#include <QPixmap>
#include <QComboBox>
#include "ui_mainwindow.h"
#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <QDebug>
#include <QGraphicsLineItem>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <qpoint.h>
#include <yaml-cpp/yaml.h>
#include "../common/map_dto.h"
#include "../common/map_loader.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>

const int TILE_SIZE = 16;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onTileSelected(int index);
    void onGridClicked(QPoint pos);
    void on_saveMapButton_clicked();

    void on_loadMapButton_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QVector<QPixmap> grassTextures;
    BlockType selectedTileIndex = Empty;
    QMap<int, QVector<QIcon>> themeTiles;
    Map map_dto;
    QPoint lastProcessedTile = QPoint(-1, -1);
    MapLoader loader;
    QPixmap currentBackground;

    void saveToYaml();

    void updateThemeSelector(int themeIndex);
    void loadThemeTiles(uint8_t theme);
    void loadTiles();
    void renderGrid();
    void placeTile(int x, int y, BlockType block_type, bool solid);
    void updateTileTexture(int x, int y);
    bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // MAINWINDOW_H

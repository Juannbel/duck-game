#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QComboBox>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QGraphicsLineItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QMainWindow>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPixmap>
#include <QString>
#include <QTextStream>
#include <QVector>

#include <qpoint.h>
#include <yaml-cpp/yaml.h>
#include "../common/map_loader.h"

#include "ui_mainwindow.h"

const int TILE_SIZE = 16;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow: public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onTileSelected(int index);
    void onGridClicked(QPoint pos);
    void on_save_mapButton_clicked();

    void on_load_mapButton_clicked();

private:
    Ui::MainWindow* ui;
    QGraphicsScene* scene;
    QVector<QPixmap> grassTextures;
    BlockType selectedTileIndex = Empty;
    QMap<int, QVector<QIcon>> themeTiles;
    MapDto map_dto;
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
    bool eventFilter(QObject* watched, QEvent* event) override;
};

#endif  // MAINWINDOW_H

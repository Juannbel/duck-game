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
#include <algorithm>

#include <qpoint.h>
#include <yaml-cpp/yaml.h>
#include "../common/map_loader.h"

#include "ui_mainwindow.h"

const int TILE_SIZE = 16;
const int DUCK_SIZE = 32;
const int GUN_SIZE = 32;
const int GUN_INDEX = 4;

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
    void onItemSelected(int index);
    void onGridClicked(QPoint pos);
    void on_save_mapButton_clicked();
    void on_load_mapButton_clicked();
    void onGridRightClicked(QPoint pos);
    void on_clear_mapButton_clicked();

private:
    Ui::MainWindow* ui;
    QGraphicsScene* scene;
    QVector<QPixmap> grassTextures;
    int selectedItemIndex = 0;
    QMap<int, QVector<QIcon>> themeTiles;
    Map map;
    QPoint lastProcessedTile = QPoint(-1, -1);
    MapLoader loader;
    QPixmap currentBackground;
    QPixmap duckTexture;
    QPixmap gunTexture;

    void saveToYaml();
    void loadDuckTexture();
    void loadGunTexture();
    void updateThemeSelector(int themeIndex);
    void loadThemeTiles(uint8_t theme);
    void loadTiles();
    void renderGrid();
    void placeTile(int x, int y, BlockType block_type, bool solid);
    bool eventFilter(QObject* watched, QEvent* event) override;
    bool validatePosition(std::pair<int16_t, int16_t> pos, bool check_blocks);
};

#endif  // MAINWINDOW_H

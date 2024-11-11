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
const int BOX_SIZE = 16;
const int BOX_SPAWN_INDEX = 14;
const int DUCK_SIZE = 32;
const int DUCK_SPAWN_INDEX = 12;
const int GUN_SIZE = 32;
const int COLLECTABLE_SPAWN_INDEX = 13;
const int AK47_POSITION = 4;

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
    void onGridClicked(QPoint pos);
    void on_saveMapButton_clicked();
    void on_loadMapButton_clicked();
    void onGridRightClicked(QPoint pos);
    void on_clearMapButton_clicked();
    void on_tileSelector_currentIndexChanged(int index);
    void on_themeSelector_currentIndexChanged(int themeIndex);

private:
    Ui::MainWindow* ui;
    QGraphicsScene* scene; 
    Map map;
    MapLoader loader;
    
    QVector<QPixmap> grassTextures;
    int selectedItemIndex = 0;
    QMap<int, QVector<QIcon>> themeTiles;
    static std::unordered_map<BlockType, QString> blockToString;
   
    QPoint lastProcessedTile = QPoint(-1, -1);
    
    QPixmap currentBackground;
    QPixmap duckTexture;
    QPixmap gunTexture;
    QPixmap boxTexture;

    void loadBoxTexture();
    void loadDuckTexture();
    void loadGunTexture();    
    void loadTiles();
    void loadThemeTiles(uint8_t theme);

    void updateThemeSelected(int theme);
    void renderGrid();
    void placeTile(int x, int y, BlockType blockType, bool solid);
    bool eventFilter(QObject* watched, QEvent* event) override;
    bool validatePosition(std::pair<int16_t, int16_t> pos, bool checkBlocks);
};

#endif  // MAINWINDOW_H

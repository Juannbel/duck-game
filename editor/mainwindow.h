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
#include <utility>

#include <qpoint.h>
#include <yaml-cpp/yaml.h>

#include "../common/map_loader.h"

#include "ui_mainwindow.h"

const int TILE_SIZE = 16;
const int BLOCKS_PER_THEME = 12;
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
    void on_saveMapButton_clicked();
    void on_loadMapButton_clicked();
    void on_clearMapButton_clicked();
    void on_itemSelector_currentIndexChanged(int index);
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

    void initialize_blocks();

    void loadBoxTexture();
    void loadDuckTexture();
    void loadGunTexture();    
    void loadTiles();
    void loadThemeTiles(uint8_t theme);
    
    void updateThemeSelected();
    void updateItemSelector();
    void updateBackground();
    void updateTilesItems();

    void renderGrid();
    void renderBackground();
    void renderGridLines();
    void renderGridTiles();
    void renderDucksSpawns();
    void renderCollectablesSpawns();
    void renderBoxesSpawns();

    bool handlerWheelEvent(QEvent* event);
    bool handlerMouseEvents(QEvent* event);
    bool handlerClickType(QPointF scenePos, Qt::MouseButton button);

    bool isPositionOccupied(std::pair<int16_t, int16_t> pos);
    bool isBlockOccupied(std::pair<int16_t, int16_t> pos);
    bool validatePosition(std::pair<int16_t, int16_t> pos, bool checkBlocks);
    
    void addDuckSpawn(std::pair<int16_t, int16_t> gridPos);
    void addCollectableSpawn(std::pair<int16_t, int16_t> gridPos);
    void addBoxSpawn(std::pair<int16_t, int16_t> gridPos);
    void addTile(std::pair<int16_t, int16_t> gridPos);
    void onGridClicked(QPoint pos);

    bool isMapValid();
    QString requestFileName();
    void onGridRightClicked(QPoint pos);
    void serializeDuckSpawns(Map& mapToSave);

    void placeTile(int x, int y, BlockType blockType, bool solid);
    bool eventFilter(QObject* watched, QEvent* event) override;
    
};

#endif  // MAINWINDOW_H

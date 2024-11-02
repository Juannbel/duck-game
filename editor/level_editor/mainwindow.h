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
#include <yaml-cpp/yaml.h>
#include "map_dto.h"
#include <QFileDialog>
#include <iostream>
#include <fstream>

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

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QVector<QPixmap> grassTextures;
    QVector<QVector<BlockType>> grid;
    int selectedThemeIndex = 0;
    BlockType selectedTileIndex = Empty;
    QMap<int, QVector<QIcon>> themeTiles;

    void saveToYaml();
    std::string blockTypeToString(BlockType type);

    void updateThemeSelector(int themeIndex);
    void loadThemeTiles(uint8_t theme);
    void loadTiles();
    void renderGrid();
    void placeTile(int x, int y, BlockType block_type);
    void updateTileTexture(int x, int y);
    bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // MAINWINDOW_H

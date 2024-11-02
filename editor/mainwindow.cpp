#include "mainwindow.h"

#include <iostream>
#include <fstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , scene(new QGraphicsScene(this))
    , grid(MAP_WIDTH_BLOCKS, QVector<BlockType>(MAP_HEIGHT_BLOCKS, Empty))
{
    ui->setupUi(this);
    ui->graphicsView->setScene(scene);

    grassTextures.resize(MAP_THEMES * 5);

    loadTiles();
    renderGrid();
    updateThemeSelector(selectedThemeIndex);

    connect(ui->tileSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onTileSelected);

    ui->graphicsView->setMouseTracking(true);
    ui->graphicsView->viewport()->installEventFilter(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::loadTiles() {

    ui->themeSelector->clear();
    ui->tileSelector->clear();

    for (int i = 0; i < MAP_THEMES; i++) {
        QString themeName = "Theme " + QString::number(i + 1);
        ui->themeSelector->addItem(themeName);
        loadThemeTiles(i);
    }

    connect(ui->themeSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::updateThemeSelector);
}

void MainWindow::loadThemeTiles(uint8_t theme) {
    QPixmap tileSet(":/images/blocks.png");
    int offset = ((MAP_THEMES + 1) * theme);
    grassTextures[static_cast<int>(Floor1) + offset] = tileSet.copy(0, theme * TILE_SIZE, TILE_SIZE, TILE_SIZE);
    grassTextures[static_cast<int>(Floor2) + offset] = tileSet.copy(TILE_SIZE, theme * TILE_SIZE, TILE_SIZE, TILE_SIZE);
    grassTextures[static_cast<int>(Floor3) + offset] = tileSet.copy(TILE_SIZE * 2, theme * TILE_SIZE, TILE_SIZE, TILE_SIZE);
    grassTextures[static_cast<int>(Base1) + offset] = tileSet.copy(TILE_SIZE * 3, theme * TILE_SIZE, TILE_SIZE, TILE_SIZE);

    QVector<QIcon> iconsForTheme = {
        QIcon(grassTextures[static_cast<int>(Empty) + offset]),
        QIcon(grassTextures[static_cast<int>(Floor1) + offset]),
        QIcon(grassTextures[static_cast<int>(Floor2) + offset]),
        QIcon(grassTextures[static_cast<int>(Floor3) + offset]),
        QIcon(grassTextures[static_cast<int>(Base1) + offset])
    };
    themeTiles[theme] = iconsForTheme;

    qDebug() << "Tiles cargados para el tema " << theme << ": " << iconsForTheme.size();
}


void MainWindow::updateThemeSelector(int themeIndex) {
    ui->tileSelector->clear();

    if (themeTiles.contains(themeIndex)) {
        auto tiles = themeTiles[themeIndex];
        ui->tileSelector->addItem(tiles[0], "Empty");
        ui->tileSelector->addItem(tiles[1], "Floor1");
        ui->tileSelector->addItem(tiles[2], "Floor2");
        ui->tileSelector->addItem(tiles[3], "Floor3");
        ui->tileSelector->addItem(tiles[4], "Base1");
        selectedThemeIndex = themeIndex;
    }
    renderGrid();
}


void MainWindow::renderGrid() {
    scene->clear();

    for (int x = 0; x <= MAP_WIDTH_BLOCKS; ++x) {
        scene->addLine(x * TILE_SIZE, 0, x * TILE_SIZE, MAP_HEIGHT_BLOCKS * TILE_SIZE, QPen(Qt::black, 1)); // Línea vertical
    }
    for (int y = 0; y <= MAP_HEIGHT_BLOCKS; ++y) {
        scene->addLine(0, y * TILE_SIZE, MAP_WIDTH_BLOCKS * TILE_SIZE, y * TILE_SIZE, QPen(Qt::black, 1)); // Línea horizontal
    }

    for (int x = 0; x < MAP_WIDTH_BLOCKS; ++x) {
        for (int y = 0; y < MAP_HEIGHT_BLOCKS; ++y) {
            if(grid[x][y] == Empty)
                continue;
            int tileIndex = static_cast<int>(grid[x][y]);
            if (tileIndex >= 0) {
                int offset = ((MAP_THEMES + 1) * selectedThemeIndex);
                QGraphicsPixmapItem *item = scene->addPixmap(grassTextures[tileIndex+offset]);
                item->setScale(1);
                item->setPos(x * TILE_SIZE, y * TILE_SIZE);
            }
        }
    }
}

void MainWindow::onTileSelected(int index) {
    selectedTileIndex = static_cast<BlockType>(index);
    qDebug() << "Tile seleccionado: " << selectedTileIndex;
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent) {
            QPoint pos = ui->graphicsView->mapFromGlobal(mouseEvent->globalPos());
            QPointF scenePos = ui->graphicsView->mapToScene(pos).toPoint();
            onGridClicked(scenePos.toPoint());
            return true;
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::onGridClicked(QPoint pos) {
    int gridX = pos.x() / (TILE_SIZE);
    int gridY = pos.y() / (TILE_SIZE);

    if (gridX >= 0 && gridX < MAP_WIDTH_BLOCKS && gridY >= 0 && gridY < MAP_HEIGHT_BLOCKS) {
        placeTile(gridX, gridY, selectedTileIndex);
        renderGrid();
        qDebug() << "Tile colocado en (" << gridX << ", " << gridY << ")";
    } else {
        qDebug() << "Clic fuera de los límites de la cuadrícula.";
    }
}

void MainWindow::placeTile(int x, int y, BlockType block_type) {
    qDebug() << "Colocando bloque "<<static_cast<int>(block_type);
    grid[x][y] = block_type;
}

void MainWindow::on_saveMapButton_clicked()
{
    saveToYaml();
}


std::string MainWindow::blockTypeToString(BlockType type){
    switch (type) {
        case Empty: return "empty";
        case Floor1: return "floor_1";
        case Floor2: return "floor_2";
        case Floor3: return "floor_3";
        case Base1: return "base_1";
        default: return "unknown";
    }
}

void MainWindow::saveToYaml() {
    YAML::Node root;
    root["width"] = 35;
    root["height"] = 20;
    root["theme"] = selectedThemeIndex;

    YAML::Node blocksNode = root["blocks"];

    for (int i = 0; i < grid.size(); ++i) {
        for (int j = 0; j < grid[i].size(); ++j) {
            BlockType block = grid[i][j];
            if (block != Empty) {
                YAML::Node blockNode;
                blockNode["x"] = i;
                blockNode["y"] = j;
                blockNode["type"] = blockTypeToString(block);
                blockNode["solid"] = true;

                blocksNode.push_back(blockNode);
            }
        }
    }

    QString filePath = QFileDialog::getSaveFileName(nullptr, "Guardar Mapa", "", "Archivos YAML (*.yaml)");
    if (filePath.isEmpty()) {
        std::cerr << "No se seleccionó ningún archivo." << std::endl;
        return;
    }

    if (!filePath.endsWith(".yaml", Qt::CaseInsensitive)) {
        filePath += ".yaml";
    }

    std::ofstream outFile(filePath.toStdString());
    outFile << root;
    outFile.close();

    std::cout << "Archivo guardado con éxito en: " << filePath.toStdString() << std::endl;

}

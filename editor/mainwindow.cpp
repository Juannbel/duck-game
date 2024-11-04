#include "mainwindow.h"

#include <iostream>
#include <QInputDialog>
#include "../common/map_loader.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , scene(new QGraphicsScene(this))
{
    map_dto.theme = 0;
    for(int i = 0; i< MAP_HEIGHT_BLOCKS;i++){
        for(int j = 0; j< MAP_WIDTH_BLOCKS;j++){
            map_dto.blocks[i][j] = {Empty,false};
        }
    }
    ui->setupUi(this);
    ui->graphicsView->setScene(scene);

    grassTextures.resize(MAP_THEMES * (static_cast<int>(HalfFloor)+1) );

    loadTiles();
    renderGrid();
    updateThemeSelector(map_dto.theme);

    connect(ui->tileSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onTileSelected);

    ui->graphicsView->setMouseTracking(true);
    ui->graphicsView->viewport()->installEventFilter(this);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing, true);
    ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
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
    int offset = ((static_cast<int>(HalfFloor) + 1) * theme);
    QVector<QIcon> iconsForTheme;
    iconsForTheme.push_back(QIcon(grassTextures[static_cast<int>(Empty) + offset]));
    for (int i = 1; i<=static_cast<int>(HalfFloor); i++ ){
        grassTextures[i + offset] = tileSet.copy(TILE_SIZE * (i-1), theme * TILE_SIZE, TILE_SIZE, TILE_SIZE);
        iconsForTheme.push_back(QIcon(grassTextures[i + offset]));
    }

    themeTiles[theme] = iconsForTheme;

    qDebug() << "Tiles cargados para el tema " << theme << ": " << iconsForTheme.size();
}


void MainWindow::updateThemeSelector(int themeIndex) {
    ui->tileSelector->clear();

    //cambiar
    std::unordered_map<BlockType, QString> block_to_string = {
                                                                  {Empty, "empty"},          {Floor1, "floor_1"},       {Floor2, "floor_2"},
                                                                  {Floor3, "floor_3"},       {Base1, "base_1"},         {Base2, "base_2"},
                                                                  {Base3, "base_3"},         {Platform1, "platform_1"}, {Platform2, "platform_2"},
                                                                  {Platform3, "platform_3"}, {Platform4, "platform_4"}, {Wall, "wall"},
                                                                  {HalfFloor, "half_floor"}};

    if (themeTiles.contains(themeIndex)) {
        auto &tiles = themeTiles[themeIndex];
        for (int i = 0; i<=static_cast<int>(HalfFloor); i++ ){
            ui->tileSelector->addItem(tiles[i], block_to_string[static_cast<BlockType>(i)]);
        }
        map_dto.theme = themeIndex;
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

    for (int y = 0; y < MAP_HEIGHT_BLOCKS ; ++y) {
        for (int x = 0; x < MAP_WIDTH_BLOCKS; ++x) {
            Block block = map_dto.blocks[y][x];
            if (block.type == Empty) {
                continue;
            }

            int tileIndex = static_cast<int>(block.type);
            int offset = (static_cast<int>(HalfFloor) + 1) * map_dto.theme;

            QGraphicsPixmapItem *item = scene->addPixmap(grassTextures[tileIndex + offset]);
            item->setPos(x * TILE_SIZE, y * TILE_SIZE);

            if (!block.solid) {
                item->setOpacity(0.5);
            }
        }
    }
}

void MainWindow::onTileSelected(int index) {
    selectedTileIndex = static_cast<BlockType>(index);
    qDebug() << "Tile seleccionado: " << selectedTileIndex;
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->graphicsView->viewport()) {
        if (event->type() == QEvent::Wheel) {
            QWheelEvent *wheelEvent = static_cast<QWheelEvent *>(event);
            if (QApplication::keyboardModifiers() == Qt::ControlModifier) {
                const double scaleFactor = 1.15;
                if (wheelEvent->angleDelta().y() > 0) {
                    ui->graphicsView->scale(scaleFactor, scaleFactor);
                } else {
                    ui->graphicsView->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
                }
                return true;
            }
        }
        else if (event->type() == QEvent::MouseButtonPress || 
                 event->type() == QEvent::MouseMove) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            
            if (mouseEvent->buttons() & Qt::LeftButton) {
                QPoint pos = ui->graphicsView->mapFromGlobal(mouseEvent->globalPos());
                QPointF scenePos = ui->graphicsView->mapToScene(pos).toPoint();
                
                QPoint currentTile(scenePos.x() / TILE_SIZE, scenePos.y() / TILE_SIZE);
                
                if (currentTile != lastProcessedTile) {
                    lastProcessedTile = currentTile;
                    onGridClicked(scenePos.toPoint());
                }
                return true;
            }
        }
        else if (event->type() == QEvent::MouseButtonRelease) {
            lastProcessedTile = QPoint(-1, -1);
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::onGridClicked(QPoint pos) {
    int gridX = pos.x() / TILE_SIZE;
    int gridY = pos.y() / TILE_SIZE;

    if (gridX >= 0 && gridX < MAP_WIDTH_BLOCKS && gridY >= 0 && gridY < MAP_HEIGHT_BLOCKS) {
        Block &block = map_dto.blocks[gridY][gridX];

        if (block.type == Empty) {
            placeTile(gridX, gridY, selectedTileIndex, true);
            qDebug() << "Nuevo bloque colocado en (" << gridX << "," << gridY << ") solid:" << map_dto.blocks[gridY][gridX].solid;
        } else if (block.type == selectedTileIndex) {
            block.solid = !block.solid;
            qDebug() << "Cambiando solidez en (" << gridX << "," << gridY << ") a:" << block.solid;
        } else {
            placeTile(gridX, gridY, selectedTileIndex, true);
            qDebug() << "Reemplazando bloque en (" << gridX << "," << gridY << ") solid:" << map_dto.blocks[gridX][gridY].solid;
        }

        renderGrid();

        qDebug() << "Estado final del bloque en (" << gridX << "," << gridY << "):"
                 << " tipo:" << static_cast<int>(map_dto.blocks[gridY][gridX].type)
                 << " solid:" << map_dto.blocks[gridY][gridX].solid;
    }
}

void MainWindow::placeTile(int x, int y, BlockType block_type, bool solid) {
    qDebug() << "Colocando bloque "<<static_cast<int>(block_type);
    map_dto.blocks[y][x] = {block_type, solid};
}

void MainWindow::on_saveMapButton_clicked()
{
    saveToYaml();
}

void MainWindow::saveToYaml() {
    bool ok;
    QString fileName = QInputDialog::getText(nullptr, "Save Map",
                                             "Insert filename (without extension):",
                                             QLineEdit::Normal, "", &ok);

    if (!ok || fileName.isEmpty()) {
        std::cerr << "No se ingresó ningún nombre de archivo." << std::endl;
        return;
    }

    if (!fileName.endsWith(".yaml", Qt::CaseInsensitive)) {
        fileName += ".yaml";
    }

    QString filePath = SERVER_DATA_PATH + static_cast<QString>("/") + fileName;

    loader.saveMap(filePath.toStdString(), map_dto);
    std::cout << "Archivo guardado con éxito en: " << filePath.toStdString() << std::endl;
}


void MainWindow::on_loadMapButton_clicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Select map to load");

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QComboBox *comboBox = new QComboBox(&dialog);
    QPushButton *loadButton = new QPushButton("Load", &dialog);

    layout->addWidget(comboBox);
    layout->addWidget(loadButton);

    std::vector<std::string> files = loader.list_maps(SERVER_DATA_PATH);

    for (const auto &file : files) {
        QString fullPath = QString::fromStdString(file);
        QFileInfo fileInfo(fullPath);
        comboBox->addItem(fileInfo.fileName());
    }

    connect(loadButton, &QPushButton::clicked, [&]() {
        if (comboBox->currentIndex() == -1) {
            QMessageBox::warning(this, "Error", "Por favor, selecciona un mapa.");
            return;
        }

        QString selectedFile = comboBox->currentText();
        if (!selectedFile.endsWith(".yaml", Qt::CaseInsensitive)) {
            selectedFile += ".yaml";
        }
        selectedFile = SERVER_DATA_PATH + static_cast<QString>("/") + selectedFile;

        Map loaded_map = loader.loadMap(selectedFile.toStdString());
        map_dto = loaded_map;
        renderGrid();
        dialog.accept();
    });

    dialog.exec();
}

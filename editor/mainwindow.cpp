#include "mainwindow.h"

#include <QInputDialog>
#include <iostream>

#include "../common/map_loader.h"

MainWindow::MainWindow(QWidget* parent):
        QMainWindow(parent), ui(new Ui::MainWindow), scene(new QGraphicsScene(this)) {
    map.map_dto.theme = 0;
    for (int i = 0; i < MAP_HEIGHT_BLOCKS; i++) {
        for (int j = 0; j < MAP_WIDTH_BLOCKS; j++) {
            map.map_dto.blocks[i][j] = {Empty, false};
        }
    }
    ui->setupUi(this);
    ui->graphicsView->setScene(scene);

    grassTextures.resize(MAP_THEMES * (static_cast<int>(HalfFloor)));

    loadTiles();
    loadDuckTexture();    
    loadGunTexture();
    renderGrid();
    updateThemeSelector(map.map_dto.theme);

    connect(ui->tileSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &MainWindow::onItemSelected);

    ui->graphicsView->setMouseTracking(true);
    ui->graphicsView->viewport()->installEventFilter(this);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing, true);
    ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::loadTiles() {

    ui->themeSelector->clear();
    ui->tileSelector->clear();

    for (int i = 0; i < MAP_THEMES; i++) {
        QString themeName = "Theme " + QString::number(i + 1);
        ui->themeSelector->addItem(themeName);
        loadThemeTiles(i);
    }

    connect(ui->themeSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &MainWindow::updateThemeSelector);
}

void MainWindow::loadThemeTiles(uint8_t theme) {
    QPixmap tileSet(":/images/blocks.png");
    int offset = ((static_cast<int>(HalfFloor)) * theme);
    QVector<QIcon> iconsForTheme;
    for (int i = 0; i < static_cast<int>(HalfFloor); i++) {
        grassTextures[i + offset] =
                tileSet.copy(TILE_SIZE * i, theme * TILE_SIZE, TILE_SIZE, TILE_SIZE);
        iconsForTheme.push_back(QIcon(grassTextures[i + offset]));
    }
    themeTiles[theme] = iconsForTheme;

    qDebug() << "Tiles cargados para el tema " << theme << ": " << iconsForTheme.size();
}

void MainWindow::loadDuckTexture(){
    QPixmap duckSprite(":/images/duck_sprite.png");
    duckTexture =
                duckSprite.copy(0, 0, DUCK_SIZE, DUCK_SIZE);
    
}    
void MainWindow::loadGunTexture(){
    QPixmap gunSprite(":/images/guns.png");
    gunTexture =
                gunSprite.copy(GUN_SIZE * GUN_INDEX , 0, GUN_SIZE, GUN_SIZE);
}

void MainWindow::updateThemeSelector(int themeIndex) {
    ui->tileSelector->clear();

    // cambiar
    std::unordered_map<BlockType, QString> block_to_string = {
            {Empty, "empty"},          {Floor1, "floor_1"},       {Floor2, "floor_2"},
            {Floor3, "floor_3"},       {Base1, "base_1"},         {Base2, "base_2"},
            {Base3, "base_3"},         {Platform1, "platform_1"}, {Platform2, "platform_2"},
            {Platform3, "platform_3"}, {Platform4, "platform_4"}, {Wall, "wall"},
            {HalfFloor, "half_floor"}};

    if (themeTiles.contains(themeIndex)) {
        auto& tiles = themeTiles[themeIndex];
        for (int i = 0; i < static_cast<int>(HalfFloor); i++) {
            ui->tileSelector->addItem(tiles[i], block_to_string[static_cast<BlockType>(i+1)]);
        }
        map.map_dto.theme = themeIndex;
    }

    ui->tileSelector->addItem(QIcon(duckTexture), "Duck");
    ui->tileSelector->addItem(QIcon(gunTexture), "Gun");
    
    QPixmap backgroundPixmap;
    backgroundPixmap.load(":/images/background_" + QString::number(themeIndex) + ".png");

    if (!backgroundPixmap.isNull()) {
        currentBackground = backgroundPixmap;
    } else {
        std::cerr << "No se pudo cargar el fondo para el tema " << themeIndex << std::endl;
    }
    renderGrid();
}


void MainWindow::renderGrid() {
    scene->clear();

    if (!currentBackground.isNull()) {

        QSize gridSize = QSize(MAP_WIDTH_BLOCKS * TILE_SIZE, MAP_HEIGHT_BLOCKS * TILE_SIZE);
        QPixmap scaledBackground = currentBackground.scaled(gridSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        QGraphicsPixmapItem *backgroundItem = scene->addPixmap(scaledBackground);
        backgroundItem->setZValue(-1);
        backgroundItem->setPos(0, 0);
    }

    for (int x = 0; x <= MAP_WIDTH_BLOCKS; ++x) {
        scene->addLine(x * TILE_SIZE, 0, x * TILE_SIZE, MAP_HEIGHT_BLOCKS * TILE_SIZE,
                       QPen(Qt::black, 1));  
    }
    for (int y = 0; y <= MAP_HEIGHT_BLOCKS; ++y) {
        scene->addLine(0, y * TILE_SIZE, MAP_WIDTH_BLOCKS * TILE_SIZE, y * TILE_SIZE,
                       QPen(Qt::black, 1)); 
    }

    for (int y = 0; y < MAP_HEIGHT_BLOCKS; ++y) {
        for (int x = 0; x < MAP_WIDTH_BLOCKS; ++x) {
            Block block = map.map_dto.blocks[y][x];
            if (block.type == Empty) {
                continue;
            }

            int tileIndex = static_cast<int>(block.type)-1;
            int offset = (static_cast<int>(HalfFloor)) * map.map_dto.theme;

            QGraphicsPixmapItem* item = scene->addPixmap(grassTextures[tileIndex + offset]);
            item->setPos(x * TILE_SIZE, y * TILE_SIZE);

            if (!block.solid) {
                item->setOpacity(0.6);
            }
        }
    }

    for (const std::pair<int16_t, int16_t>& duckPos : map.duck_spawns) {
        QPoint pos(duckPos.first, duckPos.second); 
        QGraphicsPixmapItem* duckItem = scene->addPixmap(duckTexture);
        duckItem->setPos(pos.x() * TILE_SIZE, pos.y() * TILE_SIZE);
        duckItem->setScale(0.5); 
    }

    for (const std::pair<int16_t, int16_t>& gunPos : map.collectables_spawns) {
        QPoint pos(gunPos.first, gunPos.second); 
        QGraphicsPixmapItem* gunItem = scene->addPixmap(gunTexture);
        gunItem->setPos(pos.x() * TILE_SIZE, pos.y() * TILE_SIZE);
        gunItem->setScale(0.5); 
    }
}


void MainWindow::onItemSelected(int index) {
    if (index <= static_cast<int>(HalfFloor)) {
        selectedItemIndex = static_cast<BlockType>(index);
        qDebug() << "Tile seleccionado: " << selectedItemIndex;
    } else if (index == static_cast<int>(HalfFloor) + 1) {
        selectedItemIndex = index;
        qDebug() << "Duck seleccionado";
    } else if (index == static_cast<int>(HalfFloor) + 2) {
        selectedItemIndex = index;
        qDebug() << "Gun seleccionado";
    }
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event) {
    if (watched == ui->graphicsView->viewport()) {
        if (event->type() == QEvent::Wheel) {
            QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);
            if (QApplication::keyboardModifiers() == Qt::ControlModifier) {
                const double scaleFactor = 1.15;
                if (wheelEvent->angleDelta().y() > 0) {
                    ui->graphicsView->scale(scaleFactor, scaleFactor);
                } else {
                    ui->graphicsView->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
                }
                return true;
            }
        } else if (event->type() == QEvent::MouseButtonPress ||
                   event->type() == QEvent::MouseMove) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            QPoint pos = ui->graphicsView->mapFromGlobal(mouseEvent->globalPos());
            QPointF scenePos = ui->graphicsView->mapToScene(pos).toPoint();
            if (mouseEvent->buttons() & Qt::LeftButton) {
                QPoint currentTile(scenePos.x() / TILE_SIZE, scenePos.y() / TILE_SIZE);
                if (currentTile != lastProcessedTile) {
                    lastProcessedTile = currentTile;
                    onGridClicked(scenePos.toPoint());
                }
                return true;
            }else if (mouseEvent->button() & Qt::RightButton) {
                onGridRightClicked(scenePos.toPoint()); 
                return true;
            }
        } else if (event->type() == QEvent::MouseButtonRelease) {
            lastProcessedTile = QPoint(-1, -1);
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

bool MainWindow::validatePosition(std::pair<int16_t, int16_t> pos, bool check_blocks){
    auto it = std::find(map.duck_spawns.begin(), map.duck_spawns.end(), pos);
    if(it != map.duck_spawns.end())
        return false;
    it = std::find(map.collectables_spawns.begin(), map.collectables_spawns.end(), pos);
    if(it != map.collectables_spawns.end())
        return false;
    if(check_blocks){
    Block block = map.map_dto.blocks[pos.second][pos.first];
    if (block.type != Empty) 
        return false;   
    }
    return true;
}

void MainWindow::onGridClicked(QPoint pos) {
    int gridX = pos.x() / TILE_SIZE;
    int gridY = pos.y() / TILE_SIZE;

    if (gridX >= 0 && gridX < MAP_WIDTH_BLOCKS && gridY >= 0 && gridY < MAP_HEIGHT_BLOCKS) {
        std::pair<int16_t, int16_t> gridPos = std::pair<int16_t, int16_t>(gridX, gridY);

        if(selectedItemIndex == static_cast<int>(HalfFloor)){   
            bool check_blocks = true;
            if(!validatePosition(gridPos, check_blocks))
                return;
            map.duck_spawns.push_back(gridPos);
            renderGrid();
            return;
        } else if(selectedItemIndex == static_cast<int>(HalfFloor) + 1){
            bool check_blocks = true;
            if(!validatePosition(gridPos, check_blocks))
                return;
            map.collectables_spawns.push_back(gridPos);
            renderGrid();
            return;
        }
        bool check_blocks = false;
        if(!validatePosition(gridPos, check_blocks))
            return;
        Block& block = map.map_dto.blocks[gridY][gridX];
        if (block.type == Empty) {
            placeTile(gridX, gridY, static_cast<BlockType>(selectedItemIndex), true);
            qDebug() << "Nuevo bloque colocado en (" << gridX << "," << gridY
                     << ") solid:" << map.map_dto.blocks[gridY][gridX].solid;
        } else if (block.type == (selectedItemIndex+1)) {
            block.solid = !block.solid;
            qDebug() << "Cambiando solidez en (" << gridX << "," << gridY << ") a:" << block.solid;
        } else {
            placeTile(gridX, gridY, static_cast<BlockType>(selectedItemIndex), true);
            qDebug() << "Reemplazando bloque en (" << gridX << "," << gridY
                     << ") solid:" << map.map_dto.blocks[gridX][gridY].solid;
        }

        renderGrid();

        qDebug() << "Estado final del bloque en (" << gridX << "," << gridY << "):"
                 << " tipo:" << static_cast<int>(map.map_dto.blocks[gridY][gridX].type)
                 << " solid:" << map.map_dto.blocks[gridY][gridX].solid;
    }
}

void MainWindow::onGridRightClicked(QPoint pos) {
    int gridX = pos.x() / TILE_SIZE;
    int gridY = pos.y() / TILE_SIZE;

    if (gridX >= 0 && gridX < MAP_WIDTH_BLOCKS && gridY >= 0 && gridY < MAP_HEIGHT_BLOCKS) {
        std::pair<int16_t, int16_t> gridPos(gridX, gridY);

        auto duckIt = std::find(map.duck_spawns.begin(), map.duck_spawns.end(), gridPos);
        if (duckIt != map.duck_spawns.end()) {
            map.duck_spawns.erase(duckIt);
            renderGrid();
            return;
        }

        auto gunIt = std::find(map.collectables_spawns.begin(), map.collectables_spawns.end(), gridPos);
        if (gunIt != map.collectables_spawns.end()) {
            map.collectables_spawns.erase(gunIt);
            renderGrid();
            return;
        }

        Block& block = map.map_dto.blocks[gridY][gridX];
        if (block.type != Empty) {
            block.type = Empty;
            renderGrid();
        }
    }
}



void MainWindow::placeTile(int x, int y, BlockType blockType, bool solid) {
    qDebug() << "Colocando bloque " << static_cast<int>(blockType);
    if(blockType < static_cast<int>(HalfFloor))
        map.map_dto.blocks[y][x] = {static_cast<BlockType>(blockType+1), solid};
    
}

void MainWindow::on_save_mapButton_clicked() { saveToYaml(); }

void MainWindow::saveToYaml() {
    bool ok;
    QString fileName = QInputDialog::getText(
            nullptr, "Save MapDto", "Insert filename (without extension):", QLineEdit::Normal, "",
            &ok);

    if (!ok || fileName.isEmpty()) {
        std::cerr << "No se ingresó ningún nombre de archivo." << std::endl;
        return;
    }

    if (!fileName.endsWith(".yaml", Qt::CaseInsensitive)) {
        fileName += ".yaml";
    }

    QString filePath = SERVER_DATA_PATH + static_cast<QString>("/") + fileName;

    loader.save_map(filePath.toStdString(), map);
    std::cout << "Archivo guardado con éxito en: " << filePath.toStdString() << std::endl;
}


void MainWindow::on_load_mapButton_clicked() {
    QDialog dialog(this);
    dialog.setWindowTitle("Select map to load");

    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    QComboBox* comboBox = new QComboBox(&dialog);
    QPushButton* loadButton = new QPushButton("Load", &dialog);

    layout->addWidget(comboBox);
    layout->addWidget(loadButton);

    std::vector<std::string> files = loader.list_maps(SERVER_DATA_PATH);

    for (const auto& file: files) {
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

        Map loaded_map = loader.load_map(selectedFile.toStdString());
        this->map = loaded_map;
        renderGrid();
        dialog.accept();
    });

    dialog.exec();
}

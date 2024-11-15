#include "mainwindow.h"
#include <QInputDialog>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../common/map_loader.h"

MainWindow::MainWindow(QWidget* parent):
        QMainWindow(parent), ui(new Ui::MainWindow), scene(new QGraphicsScene(this)) {

    ui->setupUi(this);
    ui->graphicsView->setScene(scene);
    map.map_dto.theme = 0;
    initialize_blocks();

    grassTextures.resize(MAP_THEMES * (static_cast<int>(HalfFloor)));

    loadTiles();
    loadDuckTexture();
    loadGunTexture();
    loadBoxTexture();
    renderGrid();
    updateThemeSelected();

    ui->saveMapButton->setIcon(QIcon::fromTheme("document-save"));
    ui->loadMapButton->setIcon(QIcon::fromTheme("document-open"));
    ui->clearMapButton->setIcon(QIcon::fromTheme("edit-delete"));
    ui->graphicsView->setMouseTracking(true);
    ui->graphicsView->viewport()->installEventFilter(this);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing, true);
    ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
}

void MainWindow::initialize_blocks(){
    for (int i = 0; i < MAP_HEIGHT_BLOCKS; i++) {
        for (int j = 0; j < MAP_WIDTH_BLOCKS; j++) {
            map.map_dto.blocks[i][j] = {Empty, false};
        }
    }
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::loadTiles() {

    ui->themeSelector->clear();
    ui->itemSelector->clear();

    for (int i = 0; i < MAP_THEMES; i++) {
        QString themeName = "Theme " + QString::number(i + 1);
        ui->themeSelector->addItem(themeName);
        loadThemeTiles(i);
    }

}

void MainWindow::loadThemeTiles(uint8_t theme) {
    QPixmap tileSet(DATA_PATH "/sprites/blocks/blocks.png");
    int offset = BLOCKS_PER_THEME * theme;
    std::vector<QIcon> iconsForTheme;
    for (int i = 0; i < BLOCKS_PER_THEME; i++) {
        grassTextures[i + offset] =
                tileSet.copy(TILE_SIZE * i, theme * TILE_SIZE, TILE_SIZE, TILE_SIZE);
        iconsForTheme.push_back(QIcon(grassTextures[i + offset]));
    }
    themeTiles[theme] = iconsForTheme;
}

void MainWindow::loadDuckTexture() {
    QPixmap duckSprite(DATA_PATH "/sprites/duck/duck_sprite.png");
    duckTexture =
                duckSprite.copy(0, 0, DUCK_SIZE, DUCK_SIZE);

}

void MainWindow::loadGunTexture(){
    QPixmap gunSprite(DATA_PATH "/sprites/guns/guns.png");
    gunTexture =
                gunSprite.copy(GUN_SIZE * AK47_POSITION , 0, GUN_SIZE, GUN_SIZE);
}

void MainWindow::loadBoxTexture(){
    QPixmap boxSprite(DATA_PATH "/sprites/boxes/boxes.png");
    boxTexture =
                boxSprite.copy(0, 0, BOX_SIZE, BOX_SIZE);
}

std::unordered_map<BlockType, QString> MainWindow::blockToString = {
        {Empty, "empty"},          {Floor1, "floor_1"},       {Floor2, "floor_2"},
        {Floor3, "floor_3"},       {Base1, "base_1"},         {Base2, "base_2"},
        {Base3, "base_3"},         {Platform1, "platform_1"}, {Platform2, "platform_2"},
        {Platform3, "platform_3"}, {Platform4, "platform_4"}, {Wall, "wall"},
        {HalfFloor, "half_floor"}};

void MainWindow::updateThemeSelected() {

    ui->itemSelector->clear();

    updateItemSelector();
    updateBackground();

    renderGrid();
}

void MainWindow::updateItemSelector() {

    updateTilesItems();

    ui->itemSelector->addItem(QIcon(duckTexture), "duck_spawn");
    ui->itemSelector->addItem(QIcon(gunTexture), "collectable_spawn");
    ui->itemSelector->addItem(QIcon(boxTexture), "collectable_box");
}

void MainWindow::updateTilesItems(){
    int themeIndex = map.map_dto.theme;
    if (themeTiles.contains(themeIndex)) {
        auto& tiles = themeTiles[themeIndex];
        for (int i = 0; i < static_cast<int>(HalfFloor); i++) {
            ui->itemSelector->addItem(tiles[i], blockToString[static_cast<BlockType>(i+1)]);
        }
    }
}

void MainWindow::updateBackground(){
    QPixmap backgroundPixmap;
    int themeIndex = map.map_dto.theme;
    backgroundPixmap.load(DATA_PATH "/backgrounds/background_" + QString::number(themeIndex) + ".png");

    if (!backgroundPixmap.isNull()) {
        currentBackground = backgroundPixmap;
    } else {
        std::cerr << "No se pudo cargar el fondo para el tema " << themeIndex << std::endl;
    }
}

void MainWindow::renderBackground(){
    if (!currentBackground.isNull()) {

        QSize gridSize = QSize(MAP_WIDTH_BLOCKS * TILE_SIZE, MAP_HEIGHT_BLOCKS * TILE_SIZE);
        QPixmap scaledBackground =
                currentBackground.scaled(gridSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        QGraphicsPixmapItem* backgroundItem = scene->addPixmap(scaledBackground);
        backgroundItem->setZValue(-1);
        backgroundItem->setPos(0, 0);
    }
}

void MainWindow::renderGridLines(){
    for (int x = 0; x <= MAP_WIDTH_BLOCKS; ++x) {
        scene->addLine(x * TILE_SIZE, 0, x * TILE_SIZE, MAP_HEIGHT_BLOCKS * TILE_SIZE,
                       QPen(Qt::black, 1));
    }
    for (int y = 0; y <= MAP_HEIGHT_BLOCKS; ++y) {
        scene->addLine(0, y * TILE_SIZE, MAP_WIDTH_BLOCKS * TILE_SIZE, y * TILE_SIZE,
                       QPen(Qt::black, 1));
    }
}



void MainWindow::renderGridTiles(){
    for (int y = 0; y < MAP_HEIGHT_BLOCKS; ++y) {
        for (int x = 0; x < MAP_WIDTH_BLOCKS; ++x) {
            Block block = map.map_dto.blocks[y][x];
            if (block.type == Empty) {
                continue;
            }

            int tileIndex = static_cast<int>(block.type) - 1;
            int offset = (static_cast<int>(HalfFloor)) * map.map_dto.theme;

            QGraphicsPixmapItem* item = scene->addPixmap(grassTextures[tileIndex + offset]);
            item->setPos(x * TILE_SIZE, y * TILE_SIZE);

            if (!block.solid) {
                item->setOpacity(0.6);
            }
        }
    }
}

void MainWindow::renderDucksSpawns(){
    for (const std::pair<int16_t, int16_t>& duckPos : map.duck_spawns) {
        QPoint pos(duckPos.first, duckPos.second);
        QGraphicsPixmapItem* duckItem = scene->addPixmap(duckTexture);
        duckItem->setPos(pos.x() * TILE_SIZE, pos.y() * TILE_SIZE);
        duckItem->setScale(0.5);
    }
}

void MainWindow::renderCollectablesSpawns(){
    for (const std::pair<int16_t, int16_t>& gunPos : map.collectables_spawns) {
        QPoint pos(gunPos.first, gunPos.second);
        QGraphicsPixmapItem* gunItem = scene->addPixmap(gunTexture);
        gunItem->setPos(pos.x() * TILE_SIZE, pos.y() * TILE_SIZE);
        gunItem->setScale(0.5);
    }
}

void MainWindow::renderBoxesSpawns(){
    for(const std::pair<int16_t, int16_t>& boxPos : map.boxes_spawns){
        QPoint pos(boxPos.first, boxPos.second);
        QGraphicsPixmapItem* boxItem = scene->addPixmap(boxTexture);
        boxItem->setPos(pos.x() * TILE_SIZE, pos.y() * TILE_SIZE);
    }
}

void MainWindow::renderGrid() {
    scene->clear();

    renderBackground();
    renderGridLines();
    renderGridTiles();

    renderDucksSpawns();
    renderCollectablesSpawns();
    renderBoxesSpawns();

}


bool MainWindow::handlerWheelEvent(QEvent* event) {
    QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);
    if (QApplication::keyboardModifiers() == Qt::ControlModifier) {
        const double scaleFactor = 1.15;
        const double minScale = 0.7;
        const double maxScale = 4.0;

        if (wheelEvent->angleDelta().y() > 0) {
            if (ui->graphicsView->transform().m11() < maxScale) {
                ui->graphicsView->scale(scaleFactor, scaleFactor);
            }
        } else {
            if (ui->graphicsView->transform().m11() > minScale) {
                ui->graphicsView->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
            }
        }
        return true;
    }
    return false;
}

bool MainWindow::handlerClickType(QPointF scenePos,Qt::MouseButton button) {
    QPoint currentTile(scenePos.x() / TILE_SIZE, scenePos.y() / TILE_SIZE);
    if (currentTile != lastProcessedTile) {
        lastProcessedTile = currentTile;
        if(button == Qt::LeftButton){
            onGridClicked(scenePos.toPoint());
        }
        else if(button == Qt::RightButton){
            onGridRightClicked(scenePos.toPoint());
        }
    }
    return true;

}

bool MainWindow::handlerMouseEvents(QEvent* event) {
    QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
    QPoint pos = ui->graphicsView->mapFromGlobal(mouseEvent->globalPos());
    QPointF scenePos = ui->graphicsView->mapToScene(pos).toPoint();
    if (mouseEvent->buttons() & Qt::LeftButton) {
        return handlerClickType(scenePos, Qt::LeftButton);
    }else if (mouseEvent->buttons() & Qt::RightButton) {
        return handlerClickType(scenePos, Qt::RightButton);
    }
    return false;
}


bool MainWindow::eventFilter(QObject* watched, QEvent* event) {
    if (watched == ui->graphicsView->viewport()) {
        if (event->type() == QEvent::Wheel) {
            if(handlerWheelEvent(event)) return true;
        } else if (event->type() == QEvent::MouseButtonPress ||
                   event->type() == QEvent::MouseMove) {
            if (handlerMouseEvents(event)) return true;
        } else if (event->type() == QEvent::MouseButtonRelease) {
            lastProcessedTile = QPoint(-1, -1);
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::on_clearMapButton_clicked() {
    map.duck_spawns.clear();
    map.collectables_spawns.clear();
    map.boxes_spawns.clear();
    for (int i = 0; i < MAP_HEIGHT_BLOCKS; i++) {
        for (int j = 0; j < MAP_WIDTH_BLOCKS; j++) {
            map.map_dto.blocks[i][j] = {Empty, false};
        }
    }
    renderGrid();
}


bool MainWindow::validatePosition(std::pair<int16_t, int16_t> pos, bool check_blocks){
    if (isPositionOccupied(pos)) return false;
    if (!check_blocks)
        return true;
    return !isBlockOccupied(pos);
}

bool MainWindow::isPositionOccupied(std::pair<int16_t, int16_t> pos) {
    if (std::find(map.duck_spawns.begin(), map.duck_spawns.end(), pos) != map.duck_spawns.end()) return true;
    if (std::find(map.collectables_spawns.begin(), map.collectables_spawns.end(), pos) != map.collectables_spawns.end()) return true;
    if (std::find(map.boxes_spawns.begin(), map.boxes_spawns.end(), pos) != map.boxes_spawns.end()) return true;
    return false;
}

bool MainWindow::isBlockOccupied(std::pair<int16_t, int16_t> pos) {
    Block block = map.map_dto.blocks[pos.second][pos.first];
    return block.type != Empty;
}

void MainWindow::addDuckSpawn(std::pair<int16_t, int16_t> gridPos) {
    if(map.duck_spawns.size() >= 4){
        QMessageBox::warning(this, "Error", "Ya hay 4 spawns de patos.");
        return;
    }
    bool check_blocks = true;
    if(!validatePosition(gridPos, check_blocks))
        return;
    map.duck_spawns.push_back(gridPos);
    renderGrid();
}

void MainWindow::addCollectableSpawn(std::pair<int16_t, int16_t> gridPos) {
    bool check_blocks = true;
    if(!validatePosition(gridPos, check_blocks))
        return;
    map.collectables_spawns.push_back(gridPos);
    renderGrid();
}

void MainWindow::addBoxSpawn(std::pair<int16_t, int16_t> gridPos) {
    bool check_blocks = true;
    if(!validatePosition(gridPos, check_blocks))
        return;
    map.boxes_spawns.push_back(gridPos);
    renderGrid();
}

void MainWindow::addTile(std::pair<int16_t, int16_t> gridPos) {
    bool check_blocks = false;
    if(!validatePosition(gridPos, check_blocks))
        return;
    Block& block = map.map_dto.blocks[gridPos.second][gridPos.first];
    if (block.type == (selectedItemIndex+1)) {
        block.solid = !block.solid;
    } else {
        placeTile(gridPos.first, gridPos.second, static_cast<BlockType>(selectedItemIndex), true);
    }
    renderGrid();
}

void MainWindow::placeTile(int x, int y, BlockType blockType, bool solid) {
    if(blockType < static_cast<int>(HalfFloor))
        map.map_dto.blocks[y][x] = {static_cast<BlockType>(blockType+1), solid};

}

void MainWindow::onGridClicked(QPoint pos) {
    int gridX = pos.x() / TILE_SIZE;
    int gridY = pos.y() / TILE_SIZE;

    if (gridX >= 0 && gridX < MAP_WIDTH_BLOCKS && gridY >= 0 && gridY < MAP_HEIGHT_BLOCKS) {
        std::pair<int16_t, int16_t> gridPos = std::pair<int16_t, int16_t>(gridX, gridY);

        if(selectedItemIndex == DUCK_SPAWN_INDEX){
            addDuckSpawn(gridPos);
            return;
        } else if(selectedItemIndex == COLLECTABLE_SPAWN_INDEX){
            addCollectableSpawn(gridPos);
            return;
        } else if(selectedItemIndex == BOX_SPAWN_INDEX){
            addBoxSpawn(gridPos);
            return;
        }else if(selectedItemIndex < DUCK_SPAWN_INDEX){
            addTile(gridPos);
            return;
        }
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

        auto gunIt =
                std::find(map.collectables_spawns.begin(), map.collectables_spawns.end(), gridPos);
        if (gunIt != map.collectables_spawns.end()) {
            map.collectables_spawns.erase(gunIt);
            renderGrid();
            return;
        }

        auto boxIt = std::find(map.boxes_spawns.begin(), map.boxes_spawns.end(), gridPos);
        if (boxIt != map.boxes_spawns.end()) {
            map.boxes_spawns.erase(boxIt);
            renderGrid();
            return;
        }

        Block& block = map.map_dto.blocks[gridY][gridX];
        if (block.type != Empty) {
            block.type = Empty;
            renderGrid();
            return;
        }
    }
}

bool MainWindow::isMapValid() {
    if(map.duck_spawns.size() != 4 ){
        QMessageBox::warning(this, "Error", "Debe haber 4 spawns de patos.");
        return false;
    }
    if(map.collectables_spawns.size() < 2 ){
        QMessageBox::warning(this, "Error", "Debe haber al menos 2 spawns de armas (collectables).");
        return false;
    }
    if(map.boxes_spawns.size() < 2 ){
        QMessageBox::warning(this, "Error", "Debe haber al menos 2 spawns de cajas.");
        return false;
    }
    return true;
}

QString MainWindow::requestFileName() {
    bool ok;
    QString fileName = QInputDialog::getText(
            nullptr, "Save MapDto", "Insert filename (without extension):", QLineEdit::Normal, "",
            &ok);

    if (!ok || fileName.isEmpty()) {
        std::cerr << "No se ingresó ningún nombre de archivo." << std::endl;
        return NULL;
    }

    if (!fileName.endsWith(".yaml", Qt::CaseInsensitive)) {
        fileName += ".yaml";
    }
    return fileName;
}

void MainWindow::serializeDuckSpawns(Map& mapToSave){
    int quantity_duck_spawns = static_cast<int>(mapToSave.duck_spawns.size());
    for(int i = 0; i < quantity_duck_spawns ; i++ ){
        mapToSave.duck_spawns[i].second -= 1;
    }
}

void MainWindow::on_saveMapButton_clicked() {

    if(!isMapValid())
        return;

    QString fileName = requestFileName();
    if (fileName.isNull()) return;

    Map mapToSave = map;
    serializeDuckSpawns(mapToSave);

    QString filePath = MAPS_PATH + static_cast<QString>("/") + fileName;

    loader.save_map(filePath.toStdString(), mapToSave);
}


void MainWindow::on_loadMapButton_clicked() {

    QDialog dialog(this);
    dialog.setWindowTitle("Select map to load");

    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    QComboBox* comboBox = new QComboBox(&dialog);
    QPushButton* loadButton = new QPushButton("Load", &dialog);

    layout->addWidget(comboBox);
    layout->addWidget(loadButton);

    std::vector<std::string> files = loader.list_maps(MAPS_PATH);

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
        selectedFile = MAPS_PATH + static_cast<QString>("/") + selectedFile;

        Map loaded_map = loader.load_map(selectedFile.toStdString());
        int quantity_duck_spawns = static_cast<int>(loaded_map.duck_spawns.size());
        for(int i = 0; i < quantity_duck_spawns ; i++ ){
            loaded_map.duck_spawns[i].second += 1;
        }
        this->map = loaded_map;
        updateThemeSelected();
        renderGrid();
        dialog.accept();
    });

    dialog.exec();
}

void MainWindow::on_itemSelector_currentIndexChanged(int index)
{
    selectedItemIndex = index;
}


void MainWindow::on_themeSelector_currentIndexChanged(int themeIndex)
{
    if (themeTiles.contains(themeIndex)) {
        map.map_dto.theme = themeIndex;
    }
    updateThemeSelected();
}

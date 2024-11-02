#include "graphic_view_scenario.h"

GraphicViewScenario::GraphicViewScenario():
        spinBoxX(nullptr), spinBoxY(nullptr), boxes(WINDOW_HEIGHT_BLOCKS * WINDOW_WIDTH_BLOCKS) {}

void GraphicViewScenario::setSpinBox(QSpinBox* spinBoxX, QSpinBox* spinBoxY) {
    this->spinBoxX = spinBoxX;
    this->spinBoxY = spinBoxY;
}

void GraphicViewScenario::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsScene::mousePressEvent(event);
    this->spinBoxX->setValue(event->scenePos().x());
    this->spinBoxY->setValue(event->scenePos().y());
}


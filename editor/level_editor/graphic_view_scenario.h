#ifndef GRAPHIC_VIEW_SCENARIO_H
#define GRAPHIC_VIEW_SCENARIO_H
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QSpinBox>
#include <QVector>

#include "editor_box.h"
#include "editor_constants.h"

class GraphicViewScenario: public QGraphicsScene {
private:
    QSpinBox* spinBoxX;
    QSpinBox* spinBoxY;
    QVector<EditorBox> boxes;

public:
    GraphicViewScenario();
    void setSpinBox(QSpinBox* spinBoxX, QSpinBox* spinBoxY);

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
};

#endif  // GRAPHIC_VIEW_SCENARIO_H

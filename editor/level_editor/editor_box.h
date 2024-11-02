#ifndef EDITOR_BOX_H
#define EDITOR_BOX_H

#include <QPointF>

#include "map_dto.h"

class EditorBox {
public:
    EditorBox();
    void set_position(int pos);
    QPointF get_scene_posicion();
    BlockType block_type;

private:
    int position;
};

#endif  // EDITOR_BOX_H

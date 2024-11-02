#include "editor_box.h"

EditorBox::EditorBox() {}

void EditorBox::set_position(int pos) { this->position = pos; }

QPointF EditorBox::get_scene_posicion() {
    QPointF scene_position;
    scene_position.setX((position % (WINDOW_WIDTH_PIXELS / WINDOW_WIDTH_BLOCKS)) *
                        WINDOW_BLOCK_SIZE);
    scene_position.setY((position / (WINDOW_WIDTH_PIXELS / WINDOW_WIDTH_BLOCKS)) *
                        WINDOW_BLOCK_SIZE);
    return scene_position;
}

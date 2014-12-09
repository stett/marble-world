#include <vector>
#include "MWEditorMenu.h"
#include "GLTextures.h"
#include "GLGraphic.h"

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

/// MWEditorMenuItem
/// 'Tors
MWEditorMenuItem::MWEditorMenuItem() {
    open_ = false;
    angle = target_angle = MENU_ITEM_ANGLE_0;
    scale = target_scale = MENU_ITEM_SCALE_0;
    alpha = target_alpha = MENU_ITEM_ALPHA_0;
}
MWEditorMenuItem::~MWEditorMenuItem() {}

/// Methods
void MWEditorMenuItem::open() {
    if (open_) return;
    open_ = true;
    angle = MENU_ITEM_ANGLE_0;
    scale = MENU_ITEM_SCALE_0;
    alpha = MENU_ITEM_ALPHA_0;
    target_angle = MENU_ITEM_ANGLE_1;
    target_scale = MENU_ITEM_SCALE_1;
    target_alpha = MENU_ITEM_ALPHA_1;
}
void MWEditorMenuItem::close() {
    open_ = false;
    target_angle = MENU_ITEM_ANGLE_2;
    target_scale = MENU_ITEM_SCALE_2;
    target_alpha = MENU_ITEM_ALPHA_0;
}
void MWEditorMenuItem::update() {

    float d_angle = target_angle - angle;
    float d_scale = target_scale - scale;
    float d_alpha = target_alpha - alpha;

    angle += d_angle * MENU_ITEM_ANGLE_D;
    scale += d_scale * MENU_ITEM_SCALE_D;
    alpha += d_alpha * MENU_ITEM_ALPHA_D;

    if (alpha < MENU_ITEM_ALPHA_MIN) alpha = 0.f;
}
void MWEditorMenuItem::draw(float x, float y) {
    if (alpha > MENU_ITEM_ALPHA_MIN) graphic.draw(x, y, angle, scale, alpha);
}

/// MWEditorMenu
/// 'Tors
MWEditorMenu::MWEditorMenu() {
    defaults();
}
MWEditorMenu::~MWEditorMenu() {}
void MWEditorMenu::defaults() {
    items.clear();
    add_item(TEX_MENU_BASE);
    add_item(TEX_MENU_PART);
    add_item(TEX_MENU_PART_LINK);
    add_item(TEX_MENU_PROP);
    add_item(TEX_MENU_MARBLE);
    add_item(TEX_MENU_SHIFT_PART);
    add_item(TEX_MENU_SHIFT_PROP);
    add_item(TEX_MENU_SHIFT_MARBLE);
}

/// Methods
void MWEditorMenu::add_item(tex texture) {
    MWEditorMenuItem item;
    item.graphic.set_texture(texture);
    items.push_back(item);
}
void MWEditorMenu::open(int menu_item_index) {
    for (int i = 0; i < (int)items.size(); i ++)
        (i == menu_item_index) ? items[i].open() : items[i].close();
}
bool MWEditorMenu::is_open() {
    for (int i = 0; i < (int)items.size(); i ++)
        if (items[i].open_) return true;
    return false;
}
void MWEditorMenu::close() {
    for (int i = 0; i < (int)items.size(); i ++)
        items[i].close();
}
void MWEditorMenu::update() {
    for (int i = 0; i < (int)items.size(); i ++)
        items[i].update();
}
void MWEditorMenu::draw(float x, float y) {
    for (int i = 0; i < (int)items.size(); i ++)
        items[i].draw(x, y);
}
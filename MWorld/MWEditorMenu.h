#ifndef _MWEDITOR_MENU_H_
#define _MWEDITOR_MENU_H_

#define MENU_BASE 0
#define MENU_PART 1
#define MENU_PART_LINK 2
#define MENU_PROP 3
#define MENU_MARBLE 4
#define MENU_SHIFT_PART 5
#define MENU_SHIFT_PROP 6
#define MENU_SHIFT_MARBLE 7

#include <vector>
#include "GLGraphic.h"
#include "GLTextures.h"
using namespace std;

#define MENU_ITEM_ANGLE_0 -90.f     // Angle that the item will open from
#define MENU_ITEM_ANGLE_1 0.f       // Angle of the item when open
#define MENU_ITEM_ANGLE_2 90.f      // Angle that the item will close to
#define MENU_ITEM_ANGLE_D .2f       // The fraction of the angle difference to make up for each step

#define MENU_ITEM_SCALE_0 .8f
#define MENU_ITEM_SCALE_1 1.f
#define MENU_ITEM_SCALE_2 1.2f
#define MENU_ITEM_SCALE_D .2f

#define MENU_ITEM_ALPHA_0 0.f
#define MENU_ITEM_ALPHA_1 .8f
#define MENU_ITEM_ALPHA_D .1f
#define MENU_ITEM_ALPHA_MIN .02f

class MWEditorMenuItem {
    /// Members
public:
    GLGraphic graphic;
    bool open_;
    float angle;
    float scale;
    float alpha;
private:
    float target_angle;
    float target_scale;
    float target_alpha;

    /// 'Tors
public:
    MWEditorMenuItem();
    ~MWEditorMenuItem();

    /// Methods
public:
    void open();
    void close();
    void update();
    void draw(float x, float y);
};

class MWEditorMenu {
    /// Members
public:
    vector<MWEditorMenuItem> items;
    
    /// 'Tors
public:
    MWEditorMenu();
    ~MWEditorMenu();
    void defaults();

    /// Methods
public:
    void add_item(tex texture);
    void open(int menu_item_index);
    bool is_open();
    void close();
    void update();
    void draw(float x, float y);
};

#endif
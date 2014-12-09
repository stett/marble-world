#ifndef _MWEDITOR_H_
#define _MWEDITOR_H_

/*
    MWEditor

    An editor instance contains a world object,
    & contains all the machinery to edit it.

    Control Scheme:

    MB-L + MB-R + Motion =      Drag view
    MB-R + Scroll =             Rotate view
    Scroll =                    Zoom view
    MB-L + Motion =             Drag component
    MB-L + Alt + Motion =       Drag component & set component angle normal to planet surface
    Control + MB-L + Motion =   Link component
    MB-L + Backspace =          Destroy component links
    MB-L + MB-R + Scroll =      Rotate component
    Shift + MB-L =              Create component
    Shift + Scroll =            Change component to be created, within category
    Shift + MB-R =              Change component category
    MB-L + Del =                Delete component
*/


#include <SDL.h>
#include "MWEditorMenu.h"
#include "MWUniversal.h"
#include "MWWorld.h"
#include "MWParts.h"
#include "MWProps.h"
using namespace std;

class MWPart;

class MWEditorSelection {
public:
    MWEditorSelection() {
        index = 0;
        selected = false;
    }
    MWWorldObjectType type;
    int index;
    bool selected;
    void select(MWWorldObjectType type_, int index_) { type = type_; index = index_; selected = true; }
    void deselect() { selected = false; }
};

struct MWEditorMouse {
    MWEditorMouse() {
        x = y = screen_x = screen_y = 0.f;
        button_left = button_right = false;
    }
    float x, y;
    float screen_x, screen_y;
    bool button_left;
    bool button_right;
};

struct MWEditorKeys {
    MWEditorKeys() {
        key_lshift = 
        key_rshift =
        key_lctrl =
        key_rctrl =
        key_lalt =
        key_ralt = false;
    }
    bool key_lshift, key_rshift;
    bool key_lctrl, key_rctrl;
    bool key_lalt, key_ralt;
};

struct MWEditorPartTemplate {
    MWEditorPartTemplate() {
        generator = 0;
    }
    void (*generator) (MWPart *part);
    MWPart instance;
};

struct MWEditorMarbleTemplate {
    MWEditorMarbleTemplate() {
        generator = 0;
    }
    void (*generator) (MWMarble *marble);
    MWMarble instance;
};

struct MWEditorPropTemplate {
    MWEditorPropTemplate() {
        generator = 0;
    }
    void (*generator) (MWProp *prop);
    MWProp instance;
};

class MWEditor {

    /// Members
public:
    MWWorld world;
    MWEditorMouse mouse;
    MWEditorKeys keys;
    MWEditorMenu menu;
    int menu_item_index;
    bool running;
    bool simulating;
    bool show_menu;
    MWEditorSelection selection;
    vector<MWEditorPartTemplate> part_templates;
    vector<MWEditorPropTemplate> prop_templates;
    vector<MWEditorMarbleTemplate> marble_templates;
    int part_template_index;
    int prop_template_index;
    int marble_template_index;
    MWWorldObjectType creation_type;

    /// 'Tors
public:
    MWEditor();
    ~MWEditor();
    void defaults();

    /// Methods
public:
    // Initialization
    void initialize();
    void initialize_sdl();
    void initialize_opengl();
    void initialize_part_templates();
    void initialize_part_template(void (*part_generator) (MWPart *part));
    void initialize_prop_templates();
    void initialize_prop_template(void (*prop_generator) (MWProp *prop));
    void initialize_marble_templates();
    void initialize_marble_template(void (*marble_generator) (MWMarble *marble));
    void initialize_menu();
    // Running
    void run();
    void update();
    void update_menu();
    void draw_world();
    void draw_menu();
    void quit();
    void gl_clear_transformations();
    // Editing
    int under_mouse(MWWorldObjectType object_type);
    MWPart *selected_part();
    MWProp *selected_prop();
    MWMarble *selected_marble();
    float angle_to_center(float x, float y);
    // SDL event handling
    void handle_events();
    void handle_key_down(SDL_keysym *keysym);
    void handle_key_up(SDL_keysym *keysym);
    void handle_mouse_down(SDL_MouseButtonEvent *button);
    void handle_mouse_up(SDL_MouseButtonEvent *button);
    void handle_mouse_motion(SDL_MouseMotionEvent *motion);
    void globalize_mouse();
};

#endif
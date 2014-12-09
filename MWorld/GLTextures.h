#ifndef _GLTEXTURES_H_
#define _GLTEXTURES_H_

#include <string>
#include <vector>

using namespace std;

struct tex {
    tex() { tex_id = 0; w = h = 0.f; }
    unsigned int tex_id;
    float w, h;
};

typedef vector<tex> tex_v;

/// Universal textures
extern tex      TEX_LINE;
extern tex      TEX_ARROW_MID;
extern tex      TEX_ARROW_HEAD;

extern tex      TEX_ROBOT_1_BALL;
extern tex      TEX_ROBOT_1_BALL_BACK;
extern tex_v    TEX_ROBOT_1_EXTEND;
extern tex_v    TEX_ROBOT_1_WALK;

extern tex      TEX_MENU_BASE;
extern tex      TEX_MENU_PART;
extern tex      TEX_MENU_PART_LINK;
extern tex      TEX_MENU_PROP;
extern tex      TEX_MENU_MARBLE;
extern tex      TEX_MENU_SHIFT_PART;
extern tex      TEX_MENU_SHIFT_PROP;
extern tex      TEX_MENU_SHIFT_MARBLE;

extern tex      TEX_SUPPORT_1_LONG;
extern tex      TEX_SUPPORT_1_SHORT;
extern tex      TEX_SUPPORT_1_TOP;

extern tex      TEX_BACKGROUND_0;
extern tex      TEX_PLANET_0;
extern tex      TEX_CLOUD_0;
extern tex      TEX_CLOUD_1;

extern tex      TEX_MARBLE_1;
extern tex      TEX_MARBLE_1_BACK;

extern tex      TEX_TRACK_1;
extern tex      TEX_TRACK_1_BACK;
extern tex      TEX_TRACK_ENTRANCE_END_1;
extern tex      TEX_TRACK_ENTRANCE_END_1_BACK;
extern tex      TEX_TRACK_ENTRANCE_MID_1;
extern tex      TEX_TRACK_ENTRANCE_MID_1_BACK;
extern tex      TEX_TRACK_CURVE45_1;
extern tex      TEX_TRACK_CURVE45_1_BACK;
extern tex      TEX_TRACK_LOOP_SMALL_QUARTER_1;
extern tex      TEX_TRACK_LOOP_SMALL_QUARTER_1_BACK;
extern tex      TEX_TRACK_LOOP_MEDIUM_EIGHTH_1;
extern tex      TEX_TRACK_LOOP_MEDIUM_EIGHTH_1_BACK;

extern tex      TEX_SWITCH_1_BACK;
extern tex_v    TEX_SWITCH_1_FLAGS;
extern tex_v    TEX_SWITCH_1;

extern tex      TEX_TRIGGER_1_BACK;
extern tex_v    TEX_TRIGGER_1;
extern tex_v    TEX_TRIGGER_1_FLAGS;

/// Functions
extern void load_all_textures();
extern tex load_texture(const string &path_str);
extern tex_v load_texture_list(const string &path_str, bool first_iteration = true);
extern void free_all_textures();

#endif
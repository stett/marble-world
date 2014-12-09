#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <boost\filesystem.hpp>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "GLTextures.h"

using namespace std;
using namespace boost::filesystem;

/// Universal textures
tex     TEX_LINE;
tex     TEX_ARROW_MID;
tex     TEX_ARROW_HEAD;

tex     TEX_ROBOT_1_BALL;
tex     TEX_ROBOT_1_BALL_BACK;
tex_v   TEX_ROBOT_1_EXTEND;
tex_v   TEX_ROBOT_1_WALK;

tex     TEX_MENU_BASE;
tex     TEX_MENU_PART;
tex     TEX_MENU_PART_LINK;
tex     TEX_MENU_PROP;
tex     TEX_MENU_MARBLE;
tex     TEX_MENU_SHIFT_PART;
tex     TEX_MENU_SHIFT_PROP;
tex     TEX_MENU_SHIFT_MARBLE;

tex     TEX_SUPPORT_1_LONG;
tex     TEX_SUPPORT_1_SHORT;
tex     TEX_SUPPORT_1_TOP;

tex     TEX_BACKGROUND_0;
tex     TEX_PLANET_0;
tex     TEX_CLOUD_0;
tex     TEX_CLOUD_1;

tex      TEX_MARBLE_1;
tex      TEX_MARBLE_1_BACK;

tex     TEX_TRACK_1;
tex     TEX_TRACK_1_BACK;
tex     TEX_TRACK_ENTRANCE_END_1;
tex     TEX_TRACK_ENTRANCE_END_1_BACK;
tex     TEX_TRACK_ENTRANCE_MID_1;
tex     TEX_TRACK_ENTRANCE_MID_1_BACK;
tex     TEX_TRACK_CURVE45_1;
tex     TEX_TRACK_CURVE45_1_BACK;
tex     TEX_TRACK_LOOP_SMALL_QUARTER_1;
tex     TEX_TRACK_LOOP_SMALL_QUARTER_1_BACK;
tex     TEX_TRACK_LOOP_MEDIUM_EIGHTH_1;
tex     TEX_TRACK_LOOP_MEDIUM_EIGHTH_1_BACK;

tex     TEX_SWITCH_1_BACK;
tex_v   TEX_SWITCH_1_FLAGS;
tex_v   TEX_SWITCH_1;

tex     TEX_TRIGGER_1_BACK;
tex_v   TEX_TRIGGER_1;
tex_v   TEX_TRIGGER_1_FLAGS;

/// Data
static tex_v all_textures;

/// Methods
void load_all_textures() {
    TEX_LINE                = load_texture("line.png");
    TEX_ARROW_MID           = load_texture("arrow_mid.png");
    TEX_ARROW_HEAD          = load_texture("arrow_head.png");

    TEX_ROBOT_1_BALL        = load_texture("graphics/robot/robot_1_ball.png");
    TEX_ROBOT_1_BALL_BACK   = load_texture("graphics/robot/robot_1_ball_back.png");
    TEX_ROBOT_1_EXTEND      = load_texture_list("graphics/robot/robot_1_extend");
    TEX_ROBOT_1_WALK        = load_texture_list("graphics/robot/robot_1_walk");

    TEX_MENU_BASE           = load_texture("graphics/EDITOR/menu_base.png");
    TEX_MENU_PART           = load_texture("graphics/EDITOR/menu_part.png");
    TEX_MENU_PART_LINK      = load_texture("graphics/EDITOR/menu_part_link.png");
    TEX_MENU_PROP           = load_texture("graphics/EDITOR/menu_prop.png");
    TEX_MENU_MARBLE         = load_texture("graphics/EDITOR/menu_marble.png");
    TEX_MENU_SHIFT_PART     = load_texture("graphics/EDITOR/menu_shift_part.png");
    TEX_MENU_SHIFT_PROP     = load_texture("graphics/EDITOR/menu_shift_prop.png");
    TEX_MENU_SHIFT_MARBLE   = load_texture("graphics/EDITOR/menu_shift_marble.png");

    TEX_SUPPORT_1_LONG      = load_texture("graphics/support_1/support_1_long.png");
    TEX_SUPPORT_1_SHORT     = load_texture("graphics/support_1/support_1_short.png");
    TEX_SUPPORT_1_TOP       = load_texture("graphics/support_1/support_1_top.png");

    TEX_BACKGROUND_0        = load_texture("graphics/atmosphere_1/atmosphere_1.tif");
    TEX_PLANET_0            = load_texture("graphics/planet_1/planet_1.png");
    TEX_CLOUD_0             = load_texture("graphics/atmosphere_1/cloud0.png");
    TEX_CLOUD_1             = load_texture("graphics/atmosphere_1/cloud1.png");

    TEX_MARBLE_1            = load_texture("graphics/marble_1/marble_1.png");
    TEX_MARBLE_1_BACK       = load_texture("graphics/marble_1/marble_1_back.png");

    TEX_TRACK_1                         = load_texture("graphics/track_1/track_1.png");
    TEX_TRACK_1_BACK                    = load_texture("graphics/track_1/track_1_back.png");
    TEX_TRACK_ENTRANCE_END_1            = load_texture("graphics/track_entrance_1/track_entrance_end_1.png");
    TEX_TRACK_ENTRANCE_END_1_BACK       = load_texture("graphics/track_entrance_1/track_entrance_end_1_back.png");;
    TEX_TRACK_ENTRANCE_MID_1            = load_texture("graphics/track_entrance_1/track_entrance_mid_1.png");
    TEX_TRACK_ENTRANCE_MID_1_BACK       = load_texture("graphics/track_entrance_1/track_entrance_mid_1_back.png");
    TEX_TRACK_CURVE45_1                 = load_texture("graphics/track_curve45_1/track_curve45_1.png");
    TEX_TRACK_CURVE45_1_BACK            = load_texture("graphics/track_curve45_1/track_curve45_1_back.png");
    TEX_TRACK_LOOP_SMALL_QUARTER_1      = load_texture("graphics/track_loop_small_1/track_loop_small_quarter_1.png");
    TEX_TRACK_LOOP_SMALL_QUARTER_1_BACK = load_texture("graphics/track_loop_small_1/track_loop_small_quarter_1_back.png");
    TEX_TRACK_LOOP_MEDIUM_EIGHTH_1      = load_texture("graphics/track_loop_medium_1/track_loop_medium_eighth_1.png");
    TEX_TRACK_LOOP_MEDIUM_EIGHTH_1_BACK = load_texture("graphics/track_loop_medium_1/track_loop_medium_eighth_1_back.png");

    TEX_SWITCH_1_BACK       = load_texture("graphics/switch_1/switch_1_back.png");
    TEX_SWITCH_1_FLAGS      = load_texture_list("graphics/switch_1/switch_1_flags");
    TEX_SWITCH_1            = load_texture_list("graphics/switch_1/switch_1");

    TEX_TRIGGER_1_BACK      = load_texture("graphics/trigger_1/trigger_1_back.png");
    TEX_TRIGGER_1           = load_texture_list("graphics/trigger_1/trigger_1");
    TEX_TRIGGER_1_FLAGS     = load_texture_list("graphics/trigger_1/trigger_1_flags");
}
tex load_texture(const string &path_str) {
    SDL_Surface *surface = IMG_Load(path_str.c_str());  // Try to load the texture into an SDL surface
    int nOfColors = surface->format->BytesPerPixel;     // Get the number of channels in the SDL surface
    GLenum texture_format;                              // Based on the pixel depth, figure out the number of channels in the surface
    if (nOfColors == 4) texture_format = GL_RGBA;       // Alpha & RGB channels
    else if (nOfColors == 3) texture_format = GL_RGB;   // No alpha channel
    tex texture;
    glGenTextures(1, &texture.tex_id);                  // Have OpenGL generate a texture object handle for us
    glBindTexture(GL_TEXTURE_2D, texture.tex_id);       // Bind the texture object & set its "stretching" properties
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, nOfColors,           // Edit the texture object's image data using the information SDL_Surface gives us
                 surface->w, surface->h,
                 0, texture_format, GL_UNSIGNED_BYTE,
                 surface->pixels);
    texture.w = (float)surface->w;                      // Save the width/height of the image
    texture.h = (float)surface->h;
    SDL_FreeSurface(surface);                           // Free the SDL surface
    all_textures.push_back(texture);
    return texture;
}
tex_v load_texture_list(const string &path_str, bool first_iteration) {
    static tex_v textures;
    if (first_iteration) textures.clear();
    path p(path_str);                                       // Create a boost path object from the path name
    if (is_regular_file(p)) {                               // Load a single file
        tex texture = load_texture(path_str);               //   Try to load the texture
        textures.push_back(texture);                        //   Add this texture to the list of textures
    } else if (is_directory(p)) {                           // Load all files in a directory to frames
        vector<path> v;                                     //   Make a path storing vector
        copy(directory_iterator(p),                         //   Fill the vector with file/subdir names
             directory_iterator(),
             back_inserter(v));
        sort(v.begin(), v.end());                           //   Sort the files we found
        for (vector<path>::const_iterator                   //   Iterate through the files
             iterator (v.begin());
             iterator != v.end();
             ++iterator) {
            if (is_regular_file(*iterator))                 //     If this entry is a regular file name, try to load it
                load_texture_list(iterator->string(), false);
        }
    }
    return textures;
}
void free_all_textures() {
    for (int i = 0; i < (int)all_textures.size(); i ++)
        glDeleteTextures(1, &all_textures[i].tex_id);
}
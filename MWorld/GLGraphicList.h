#ifndef _GLGRAPHICLIST_H_
#define _GLGRAPHICLIST_H_

#include <vector>
#include <string>
#include "GLTextures.h"

using namespace std;

class GLGraphicList {

    /// Members
public:
    tex_v textures;

    /// 'Tors
public:
    GLGraphicList();
    GLGraphicList(const GLGraphicList &graphiclist);
    GLGraphicList(const tex_v &textures_);
    ~GLGraphicList();
    void defaults();

    /// Methods
public:
    void set_textures(const tex_v &textures_);
    void set_texture(const tex &texture_);
    void add_frame(const tex &texture_);
    inline void draw(int frame);
    void draw(int frame, float x, float y);
    void draw(int frame, float x, float y, float angle);
    void draw(int frame, float x, float y, float angle, float scale);
    void copy(const GLGraphicList &graphiclist);

    /// Operators
public:
    GLGraphicList& operator=(const GLGraphicList &rhs);
};

#endif
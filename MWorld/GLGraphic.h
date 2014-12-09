/*
    GLGraphic
    Loads an OpenGL texture & contains simple methods for drawing,
    to be used within the context of an OpenGL scene rendering
*/

#ifndef _GLGRAPHIC_H_
#define _GLGRAPHIC_H_

#include "GLTextures.h"

class GLGraphic {

    /// Members
public:
    float w_half, h_half;
    tex texture;

    /// 'Tors
public:
    GLGraphic();
    GLGraphic(const GLGraphic &graphic);
    GLGraphic(tex texture_);
    ~GLGraphic();
    void defaults();

    /// Methods
public:
    void set_texture(const tex &texture_);
    inline void draw();
    void draw(float x, float y);
    void draw(float x, float y, float angle);
    void draw(float x, float y, float angle, float scale);
    void draw(float x, float y, float angle, float scale, float alpha);
    void copy(const GLGraphic &graphic);

    /// Operators
public:
    GLGraphic& operator=(const GLGraphic &rhs);
};

#endif
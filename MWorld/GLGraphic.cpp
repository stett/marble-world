#include "GLGraphic.h"

#include <SDL.h>
#include <SDL_image.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "GLTextures.h"

using namespace std;

/// 'Tors
GLGraphic::GLGraphic() {
    defaults();
}
GLGraphic::GLGraphic(const GLGraphic &graphic) {
    copy(graphic);
}
GLGraphic::GLGraphic(tex texture_) {
    defaults();
    texture = texture_;
    w_half = texture.w * .5f;
    h_half = texture.h * .5f;
}
GLGraphic::~GLGraphic() {}
void GLGraphic::defaults() {
    w_half = h_half = 0.f;
}

/// Methods
void GLGraphic::set_texture(const tex &texture_) {
    texture = texture_;
    w_half = texture.w * .5f;
    h_half = texture.h * .5f;
}
inline void GLGraphic::draw() {
    glBindTexture(GL_TEXTURE_2D, texture.tex_id);
    glBegin(GL_QUADS);
        glTexCoord2i(0, 0);                 // Top-left
        glVertex2f(-w_half, -h_half);
        glTexCoord2i(1, 0);                 // Top-right
        glVertex2f(w_half, -h_half);
        glTexCoord2i(1, 1);                 // Bottom-right
        glVertex2f(w_half, h_half);
        glTexCoord2i(0, 1);                 // Bottom-left
        glVertex2f(-w_half, h_half);
    glEnd();
}
void GLGraphic::draw(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0.f);
    draw();
    glPopMatrix();
}
void GLGraphic::draw(float x, float y, float angle) {
    glPushMatrix();
    glTranslatef(x, y, 0.f);
    glRotatef(angle, 0.f, 0.f, 1.f);
    draw();
    glPopMatrix();
}
void GLGraphic::draw(float x, float y, float angle, float scale) {
    glPushMatrix();
    glTranslatef(x, y, 0.f);
    glRotatef(angle, 0.f, 0.f, 1.f);
    glScalef(scale, scale, 0.f);
    draw();
    glPopMatrix();
}
void GLGraphic::draw(float x, float y, float angle, float scale, float alpha) {
    glPushMatrix();
    glTranslatef(x, y, 0.f);
    glRotatef(angle, 0.f, 0.f, 1.f);
    glScalef(scale, scale, 0.f);
    glColor4f(1.f, 1.f, 1.f, alpha);
    draw();
    glColor4f(1.f, 1.f, 1.f, 1.f);
    glPopMatrix();
}
void GLGraphic::copy(const GLGraphic &graphic) {
    texture = graphic.texture;
    w_half = graphic.w_half;
    h_half = graphic.h_half;
}

/// Operators
GLGraphic& GLGraphic::operator=(const GLGraphic &rhs) {
    if (this != &rhs) copy(rhs);
    return *this;
}
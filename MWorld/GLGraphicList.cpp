#include "GLGraphicList.h"

#include <SDL.h>
#include <SDL_image.h>
#include <boost\filesystem.hpp>
#include <vector>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;
using namespace boost::filesystem;

/// 'Tors
GLGraphicList::GLGraphicList() {
    defaults();
}
GLGraphicList::GLGraphicList(const GLGraphicList &graphiclist) {
    defaults();
    copy(graphiclist);
}
GLGraphicList::GLGraphicList(const tex_v &textures_) {
    defaults();
    textures = textures_;
}
GLGraphicList::~GLGraphicList() {
}
void GLGraphicList::defaults() {
}

/// Methods
void GLGraphicList::set_textures(const tex_v &textures_) {
    textures = textures_;
}
void GLGraphicList::set_texture(const tex &texture_) {
    textures.clear();
    textures.push_back(texture_);
}
void GLGraphicList::add_frame(const tex &texture_) {
    textures.push_back(texture_);
}
inline void GLGraphicList::draw(int frame) {
    if (frame < 0 || frame >= (int)textures.size()) return;
    float w_half = textures[frame].w * .5f;
    float h_half = textures[frame].h * .5f;
    glBindTexture(GL_TEXTURE_2D, textures[frame].tex_id);
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
void GLGraphicList::draw(int frame, float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0.f);
    draw(frame);
    glPopMatrix();
}
void GLGraphicList::draw(int frame, float x, float y, float angle) {
    glPushMatrix();
    glTranslatef(x, y, 0.f);
    glRotatef(angle, 0.f, 0.f, 1.f);
    draw(frame);
    glPopMatrix();
}
void GLGraphicList::draw(int frame, float x, float y, float angle, float scale) {
    glPushMatrix();
    glTranslatef(x, y, 0.f);
    glRotatef(angle, 0.f, 0.f, 1.f);
    glScalef(scale, scale, 0.f);
    draw(frame);
    glPopMatrix();
}
void GLGraphicList::copy(const GLGraphicList &graphiclist) {
    textures = graphiclist.textures;
}

/// Operators
GLGraphicList& GLGraphicList::operator=(const GLGraphicList &rhs) {
    if (this != &rhs) copy(rhs);
    return *this;
}
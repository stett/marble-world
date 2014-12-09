#include "MWMarble.h"
#include "GLDraw.h"

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/// 'Tors
MWMarble::MWMarble() {
    defaults();
}
MWMarble::~MWMarble() {
}
void MWMarble::defaults() {
    m = 1.f;
    r = 35.f;
    e = .99f;
    x = y = 0.f;
    vx = vy = 0.f;
    angle_fore = angle_back = 0.f;
    angle_v = 0.f;
    pathed = false;
}

/// Methods
void MWMarble::draw() {
    glPushMatrix();
    graphic_back.draw(x, y, angle_back);
    graphic_fore.draw(x, y, angle_fore);
    glPopMatrix();
}
void MWMarble::draw(float x_, float y_) {
    glPushMatrix();
    graphic_back.draw(x_, y_, angle_back);
    graphic_fore.draw(x_, y_, angle_fore);
    glPopMatrix();
}
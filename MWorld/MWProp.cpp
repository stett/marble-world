#include "MWUniversal.h"
#include "MWProp.h"
#include "GLDraw.h"

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void MWPropConnectionVertexSet::globalize() {
    static GLint viewport[4];
    static GLdouble modelview[16], projection[16];
    static GLfloat wx, wy;
    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);

    wx = (GLfloat)vertex_local.x;
    wy = (GLfloat)(viewport[3] - vertex_local.y);

    static double x, y, z;
    gluUnProject(wx, wy, 0, modelview, projection, viewport, &x, &y, &z);
    vertex_global.x = (float)x;
    vertex_global.y = (float)y;
}

/// 'Tors
MWProp::MWProp() {
    defaults();
}
MWProp::~MWProp() {
}
void MWProp::defaults() {
    x = y = 0.f;
    angle = 0.f;
    r = 0.f;
    update = &MWPropDefaults::update;
}

/// Methods
void MWProp::draw() {
    glPushMatrix();
    glTranslatef(x, y, 0.f);
    glRotatef(angle, 0.f, 0.f, 1.f);
    
    // Draw the graphic
    static int i;
    for (i = 0; i < (int)graphics.size(); i ++) graphics[i].draw();

    glPopMatrix();
}
void MWProp::update_radius() {
    r = 0.f;
    static float x;
    static float y;
    for (int i = 0; i < (int)connection_vertices.size(); i ++) {
        x = connection_vertices[i].vertex_local.x;
        y = connection_vertices[i].vertex_local.y;
        r = max(r, sqrt(x*x + y*y));
    }
}
void MWProp::globalize_connection_vertices() {

    glPushMatrix();
    glRotatef(-angle, 0.f, 0.f, 1.f);
    glTranslatef(-x, -y, 0.f);

    for (int i = 0; i < (int)connection_vertices.size(); i ++)
        connection_vertices[i].globalize();
    /*
    static GLint viewport[4];
    static GLdouble modelview[16], projection[16];
    static GLfloat wx, wy;
    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);

    for (int i = 0; i < (int)connection_vertices.size(); i ++) {

        wx = (GLfloat)connection_vertices[i].vertex_local.x;
        wy = (GLfloat)(viewport[3] - connection_vertices[i].vertex_local.y);

        static double x, y, z;
        gluUnProject(wx, wy, 0, modelview, projection, viewport, &x, &y, &z);
        connection_vertices[i].vertex_global.x = (float)x;
        connection_vertices[i].vertex_global.y = (float)y;
    }
    */

    glPopMatrix();
}

/// Default instance functions
namespace MWPropDefaults {
    void update(MWProp *prop) {
        return;
    }
};
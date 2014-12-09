#include "MWPath.h"
#include "MWMath.h"

#include <vector>
#include <stdarg.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "GLDraw.h"

using namespace std;

/// 'Tors
MWPath::MWPath() {
    defaults();
}
MWPath::~MWPath() {
}
void MWPath::defaults() {
    r = 0;
}

/// Methods
int MWPath::add_vertex(float x, float y, MWPathVertexOpenSide open_side) {
    vertices.push_back(MWPathVertex(x, y, open_side));
    update_radius();
    update_normals();
    return (int)vertices.size() - 1;
}
void MWPath::draw() {
    int vertex_index0, vertex_index1;

    // Draw vertices
    for (vertex_index0 = 0; vertex_index0 < (int)vertices.size(); vertex_index0 ++) {
        GLDraw_line(vertices[vertex_index0].x - 10, vertices[vertex_index0].y - 10,
                    vertices[vertex_index0].x + 10, vertices[vertex_index0].y + 10,
                    2.f, 1.f, 1.f, 0.f, .5f);
        GLDraw_line(vertices[vertex_index0].x - 10, vertices[vertex_index0].y + 10,
                    vertices[vertex_index0].x + 10, vertices[vertex_index0].y - 10,
                    2.f, 1.f, 1.f, 0.f, .5f);
    }
    // Draw paths
    for (vertex_index1 = 1; vertex_index1 < (int)vertices.size(); vertex_index1 ++) {
        vertex_index0 = vertex_index1 - 1;
        GLDraw_line(vertices[vertex_index0].x, vertices[vertex_index0].y,
                    vertices[vertex_index1].x, vertices[vertex_index1].y,
                    5.f, .5f, .5f, 1.f, .8f);

        if (vertices[vertex_index0].open_side != NoSide) {
            float dx = vertices[vertex_index1].x - vertices[vertex_index0].x;
            float dy = vertices[vertex_index1].y - vertices[vertex_index0].y;
            float nx, ny;
            float d_max = sqrt(dx*dx + dy*dy);
            MWMath_normalize(dx, dy, &nx, &ny);
            float dash_length = 10.f;
            for (float d = 0.f; d < d_max; d += 2 * dash_length) {
                float ox = -ny * 10.f * (vertices[vertex_index0].open_side == RightSide ? 1.f : -1.f);
                float oy = nx * 10.f * (vertices[vertex_index0].open_side == RightSide ? 1.f : -1.f);
                float x0 = vertices[vertex_index0].x + ox + nx * d;
                float y0 = vertices[vertex_index0].y + oy + ny * d;
                float x1 = vertices[vertex_index0].x + ox + nx * (d + dash_length);
                float y1 = vertices[vertex_index0].y + oy + ny * (d + dash_length);

                GLDraw_line(x0, y0, x1, y1, 4.f, .7f, .5f, 1.f, .8f);
            }
        }
    }
}
void MWPath::draw(float x, float y, float angle) {
    glPushMatrix();
    glTranslatef(x, y, 0.f);
    glRotatef(angle, 0.f, 0.f, 1.f);
    draw();
    glPopMatrix();
}
void MWPath::update_radius() {
    r = 0;
    for (int i = 0; i < (int)vertices.size(); i ++)
        r = max(r, sqrt(vertices[i].x * vertices[i].x + vertices[i].y * vertices[i].y));
}
void MWPath::update_normals() {
    MWPathVertex *v0 = 0;
    MWPathVertex *v1 = 0;
    float mag, mag_inv, dx, dy;

    for (int i = 1; i < (int)vertices.size(); i ++) {
        v0 = &vertices[i-1];
        v1 = &vertices[i];
        dx = v1->x - v0->x;
        dy = v1->y - v0->y;
        mag = sqrt(dx*dx + dy*dy);
        mag_inv = 1.f / mag;
        v0->l = mag;
        v0->nx = dx * mag_inv;
        v0->ny = dy * mag_inv;
    }
    if (v0 && v1) {
        v1->l = 0.f;
        v1->nx = v0->nx;
        v1->ny = v0->ny;
    } else {
        v0 = &vertices[0];
        v0->l = 0.f;
        v0->nx = 0.f;
        v0->ny = 0.f;
    }
}
void MWPath::globalize_coordinates() {

    static GLint viewport[4];
    static GLdouble modelview[16], projection[16];
    static GLfloat wx, wy;
    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);

    for (int i = 0; i < (int)vertices.size(); i ++) {

        wx = (GLfloat)vertices[i].x;
        wy = (GLfloat)(viewport[3] - vertices[i].y);

        static double x, y, z;
        gluUnProject(wx, wy, 0, modelview, projection, viewport, &x, &y, &z);
        vertices[i].x = (float)x;
        vertices[i].y = (float)y;
    }

    update_normals();
}
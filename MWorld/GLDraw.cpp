#include <math.h>
#include <algorithm>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "GLDraw.h"
#include "GLTextures.h"

using namespace std;

void GLDraw_line(float x0, float y0, float x1, float y1, float width, float R, float G, float B, float A) {
    static float angle;
    static float dx, dy;
    static float length2;
    static float width2;

    dx = x1 - x0;
    dy = y1 - y0;
    angle = 57.2957795f * atan2(dy, dx);
    length2 = .5f * sqrt(dx*dx + dy*dy);
    width2 = .5f * width;

    glPushMatrix();
    glTranslatef(.5f * (x0 + x1), .5f * (y0 + y1), 0.f);
    glRotatef(angle, 0.f, 0.f, 1.f);

    glColor4f(R, G, B, A);

    glBindTexture(GL_TEXTURE_2D, TEX_LINE.tex_id);
    glBegin(GL_QUADS);
        glTexCoord2i(0, 0);                 // Top-left
        glVertex2f(-length2, -width2);
        glTexCoord2i(1, 0);                 // Top-right
        glVertex2f(length2, -width2);
        glTexCoord2i(1, 1);                 // Bottom-right
        glVertex2f(length2, width2);
        glTexCoord2i(0, 1);                 // Bottom-left
        glVertex2f(-length2, width2);
    glEnd();

    glPopMatrix();

    glColor4f(1.f, 1.f, 1.f, 1.f);
}
void GLDraw_circle(float r, float width, float R, float G, float B, float A) {
    float px0, py0;
    float px1, py1;
    int points = (int)(r / 8.f);
    if (points < 8) points = 8;
    if (points > 25) points = 25;
    float increment = 2.f * 3.14159265f / (float)points;
    for (float a = 0.f; a < 2.f * 3.14159265f; a += increment) {
        px0 = cos(a) * r;
        py0 = sin(a) * r;
        if (a > 0.f)
            GLDraw_line(px0, py0, px1, py1, width, R, G, B, A);
        px1 = px0;
        py1 = py0;
    }
    GLDraw_line(r, 0.f, px1, py1, width, R, G, B, A);
}
void GLDraw_circle(float x, float y, float r, float width, float R, float G, float B, float A) {
    glPushMatrix();
    glTranslatef(x, y, 0.f);
    GLDraw_circle(r, width, R, G, B, A);
    glPopMatrix();
}
void GLDraw_arrow(float x0, float y0, float x1, float y1, float width, float R, float G, float B, float A) {

    static float angle;
    static float dx, dy;
    static float length2;
    static float width2;

    dx = x1 - x0;
    dy = y1 - y0;
    angle = 57.2957795f * atan2(dy, dx);
    length2 = max(TEX_ARROW_HEAD.w, .5f * sqrt(dx*dx + dy*dy));
    width2 = .5f * width;

    glPushMatrix();
    glTranslatef(.5f * (x0 + x1), .5f * (y0 + y1), 0.f);
    glRotatef(angle, 0.f, 0.f, 1.f);

    glColor4f(R, G, B, A);

    // Draw the middle section
    glBindTexture(GL_TEXTURE_2D, TEX_ARROW_MID.tex_id);
    glBegin(GL_QUADS);
        glTexCoord2i(0, 0);                 // Top-left
        glVertex2f(-length2, -width2);
        glTexCoord2i(1, 0);                 // Top-right
        glVertex2f(length2 - TEX_ARROW_HEAD.w, -width2);
        glTexCoord2i(1, 1);                 // Bottom-right
        glVertex2f(length2 - TEX_ARROW_HEAD.w, width2);
        glTexCoord2i(0, 1);                 // Bottom-left
        glVertex2f(-length2, width2);
    glEnd();

    // Draw the head
    glBindTexture(GL_TEXTURE_2D, TEX_ARROW_HEAD.tex_id);
    glBegin(GL_QUADS);
        glTexCoord2i(0, 0);                 // Top-left
        glVertex2f(length2 - TEX_ARROW_HEAD.w, -width2);
        glTexCoord2i(1, 0);                 // Top-right
        glVertex2f(length2, -width2);
        glTexCoord2i(1, 1);                 // Bottom-right
        glVertex2f(length2, width2);
        glTexCoord2i(0, 1);                 // Bottom-left
        glVertex2f(length2 - TEX_ARROW_HEAD.w, width2);
    glEnd();

    glPopMatrix();

    glColor4f(1.f, 1.f, 1.f, 1.f);
}
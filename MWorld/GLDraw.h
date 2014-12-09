#ifndef _GLDRAW_H_
#define _GLDRAW_H_

extern void GLDraw_line(float x0, float y0, float x1, float y1, float width=1.f, float R=1.f, float G=1.f, float B=1.f, float A=1.f);
extern void GLDraw_circle(float r, float width, float R=1.f, float G=1.f, float B=1.f, float A=1.f);
extern void GLDraw_circle(float x, float y, float r, float width, float R, float G, float B, float A=1.f);
extern void GLDraw_arrow(float x0, float y0, float x1, float y1, float width=32.f, float R=1.f, float G=1.f, float B=1.f, float A=1.f);

#endif
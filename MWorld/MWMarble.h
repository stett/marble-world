#ifndef _MWMARBLE_H_
#define _MWMARBLE_H_

#include "GLGraphic.h"

class MWMarble {

    /// Members
public:
    // Model data
    GLGraphic graphic_fore;
    GLGraphic graphic_back;
    float m;
    float r;
    float e;
    // Instance data
    float x, y;
    float vx, vy;
    float angle_fore;
    float angle_back;
    float angle_v;
    bool pathed;

    /// 'Tors
public:
    MWMarble();
    ~MWMarble();
    void defaults();

    /// Methods
public:
    void draw();
    void draw(float x_, float y_);
};

#endif
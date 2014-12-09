#ifndef _MWPROP_H_
#define _MWPROP_H_

#include "GLGraphicAnimation.h"

struct MWPropConnectionVertex {
    MWPropConnectionVertex() { x = y = 0.f; }
    float x, y;
};

class MWPropConnectionVertexSet {
public:
    MWPropConnectionVertex vertex_local;
    MWPropConnectionVertex vertex_global;
    void globalize();
};

class MWProp {

    /// Members
public:
    // Model data
    vector<GLGraphicAnimation> graphics;
    vector<MWPropConnectionVertexSet> connection_vertices;
    // Instance data
    float x, y;
    float angle;
    float r;
    // Instance functions
    void (*update) (MWProp *prop);

    /// 'Tors
public:
    MWProp();
    ~MWProp();
    void defaults();

    /// Methods
public:
    void draw();
    void update_radius();
    void globalize_connection_vertices();
};

/// Default instance functions
namespace MWPropDefaults {
    extern void update(MWProp *prop);
};

#endif
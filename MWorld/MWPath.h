#ifndef _MWPATH_H_
#define _MWPATH_H_

#include <vector>

using namespace std;

class MWPart;

enum MWPathVertexOpenSide { NoSide, LeftSide, RightSide };
struct MWPathVertex {
    MWPathVertex() { x = y = nx = ny = l = 0; open_side = NoSide; }
    MWPathVertex(float x_, float y_) { x = x_; y = y_; nx = ny = l = 0; open_side = NoSide; }
    MWPathVertex(float x_, float y_, MWPathVertexOpenSide open_side_) { x = x_; y = y_; nx = ny = l = 0; open_side = open_side_; }
    float x, y;
    float nx, ny;
    float l;
    MWPathVertexOpenSide open_side;
};

class MWPath {

    /// Members
public:
    vector<MWPathVertex> vertices;
    float r;

    /// 'Tors
public:
    MWPath();
    ~MWPath();
    void defaults();

    /// Methods
public:
    int add_vertex(float x, float y, MWPathVertexOpenSide open_side = NoSide);
    void draw();
    void draw(float x, float y, float angle);
    void update_radius();
    void update_normals();
    void globalize_coordinates();
};

#endif
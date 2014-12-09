#ifndef _MWPART_H_
#define _MWPART_H_

#include <vector>
#include "MWUniversal.h"
#include "MWPath.h"
#include "MWProp.h"
#include "MWRobot.h"
#include "GLGraphicAnimation.h"

using namespace std;

class MWMarble;
class MWWorld;

enum MWPartMarbleType { PathedMarble, PathedRobot };
struct MWPartMarble {
    MWPartMarble() {
        index = -1;
        path_index = -1;
        vertex_index = -1;
        path_position = 0;
        path_velocity = 0;
    }
    MWPartMarbleType type;
    int index;
    int path_index;
    int vertex_index;
    float path_position;
    float path_velocity;
};

typedef vector<MWPath> path_vec;

struct MWPartEntrance {
    MWPartEntrance(int vertex_index_ = 0, int path_index_ = 0, float radius_ = 50.f, float inner_radius_ = 15.f, float suction_ = .3f) {
        path_index = path_index_;
        vertex_index = vertex_index_;
        radius = radius_;
        inner_radius = inner_radius_;
        suction = suction_;
    }
    int path_index;
    int vertex_index;
    float radius;
    float inner_radius;
    float suction;
};

struct MWPartConnection {
    MWPartConnection() {
        part_index_other =
        path_set_index_other =
        path_index[0] = path_index[1] =
        vertex_index[0] = vertex_index[1] = -1;
    }
    MWPartConnection(int part_index_other_, int path_set_index_other_, int path_index_0, int path_index_1, int vertex_index_0, int vertex_index_1) {
        part_index_other = part_index_other_;
        path_set_index_other = path_set_index_other_;
        path_index[0] = path_index_0;
        path_index[1] = path_index_1;
        vertex_index[0] = vertex_index_0;
        vertex_index[1] = vertex_index_1;
    }
    int part_index_other;
    int path_set_index_other;
    int path_index[2];
    int vertex_index[2];
};

struct MWPartPathSet {
    path_vec paths_local;
    path_vec paths_global;
    vector<MWPartEntrance> entrances;
    vector<MWPartConnection> connections;
};

class MWPart {

    /// Members
public:
    // Model data
    vector<MWPartMarble> marbles;
    vector<GLGraphicAnimation> graphics_back;
    vector<GLGraphicAnimation> graphics_fore;
    vector<MWPartPathSet> path_sets;
    int path_set_index;
    vector<MWPropConnectionVertexSet> connection_vertices;
    // Instance data
    float x, y;
    float angle;
    float r;
    int id;
    MWWorld *world;
    vector<int> link_part_indices;
    // Part type function pointers
    void (*update) (MWPart *part);
    void (*marble_in) (MWPart *part, const MWPartMarble &marble_container);
    void (*marble_out) (MWPart *part, int marble_index);
    void (*signal_recieve) (MWPart *part, MWWorldObjectType type, int value);

    /// 'Tors
public:
    MWPart();
    ~MWPart();
    void defaults();

    /// Methods
public:
    void draw_back();
    void draw_fore();
    void draw_marbles();
    void update_radius();
    void globalize_coordinates();
    void connect_by_proximity();
    void disconnect();
    MWPartConnection *get_connection(int path_set_index_, int path_index, int vertex_index, bool return_0_if_currently_invalid = true);
    path_vec *path_set_local();
    path_vec *path_set_global();
    MWMarble *marble(int marble_local_index);
    MWRobot *robot(int marble_local_index);
};

/// Default part type functions
namespace MWPartDefaults {
    extern void part_update(MWPart *part);
    extern void part_update_graphics(MWPart *part);
    extern void part_update_marbles(MWPart *part);
    extern void part_marble_in(MWPart *part, const MWPartMarble &marble_container);
    extern void part_marble_out(MWPart *part, int marble_local_index);
    extern void signal_recieve(MWPart *part, MWWorldObjectType type, int value);
}

#endif
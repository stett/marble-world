#ifndef _MWWORLD_H_
#define _MWWORLD_H_

#include <vector>
#include "MWUniversal.h"
#include "GLGraphic.h"
#include "MWMarble.h"
#include "MWRobot.h"
#include "MWPart.h"
#include "MWProp.h"

using namespace std;

struct MWWorldView {
    float x, y;
    float angle;
    float scale;
};

struct MWWorldAtmosphere {
    MWWorldAtmosphere(const tex &texture) { angle = 0.f; graphic.set_texture(texture); }
    MWWorldAtmosphere(const GLGraphic &graphic_) { angle = 0.f; graphic = graphic_; }
    MWWorldAtmosphere() { angle = 0.f; }
    GLGraphic graphic;
    float angle;
};

class MWWorld {

    /// Members
public:
    // Model data
    vector<MWRobot> robots;
    vector<MWMarble> marbles;
    vector<MWPart> parts;
    vector<MWProp> props;
    GLGraphic graphic_planet;
    vector<MWWorldAtmosphere> atmosphere_back;
    vector<MWWorldAtmosphere> atmosphere_fore;
    GLGraphic atmosphere_sky;
    float planet_radius;
    float gravitation;
    float lightness_parts;
    float lightness_props;
    // Instance data
    MWWorldView view;
    // Instance function pointers
    void (*update_atmosphere) (MWWorld *world);
    void (*draw_back) (MWWorld *world);
    void (*draw_fore) (MWWorld *world);
    // Callback functions for instances in the world
    void (*signal_recieve) (MWWorld *world, MWWorldObjectType type, int value);
private:
    // Internal management data
    vector<int> globalize_part_indices;
    vector<int> globalize_prop_indices;

    /// 'Tors
public:
    MWWorld();
    ~MWWorld();
    void defaults();

    /// Methods
public:
    // Updating
    void update();
    void update_globalization();
    void update_part_globalization(int part_index);
    void update_prop_globalization(int prop_index);
    void update_robots();
    void update_marbles();
    void update_parts();
    void update_props();
    void update_part_indices();
    bool enpath(int *part_index, MWPartMarble *marble_container,
                MWPartMarbleType type, int index,
                float &marble_x, float &marble_y,
                float &marble_vx, float &marble_vy,
                float marble_r);
    // Drawing
    void draw();
    void view_transformations();
    void view_inverse_transformations();
    // Part management
    int add_part(void (*part_generator) (MWPart *part), float x = 0.f, float y = 0.f, float angle = 0.f);
    int add_marble(void (*marble_generator) (MWMarble *marble), float x = 0.f, float y = 0.f);
    int add_prop(void (*prop_generator) (MWProp *prop), float x = 0.f, float y = 0.f, float angle = 0.f);
    void remove_part(int part_index);
    void remove_marble(int marble_index);
    void remove_prop(int prop_index);
    void connect_parts(int part_index_0, int path_set_index_0, int path_index_0, int vertex_index_0, 
                       int part_index_1, int path_set_index_1, int path_index_1, int vertex_index_1,
                       bool connect_overlapping_vertices = true);
    // Getters
    MWRobot *player();
    // Signal management
    void signal_send(MWWorldObjectType type, int reciever_id, int value);
};

/// Default instance & callback functions
namespace MWWorldDefaults {
    extern void update_atmosphere(MWWorld *world);
    extern void draw_back(MWWorld *world);
    extern void draw_fore(MWWorld *world);
    extern void signal_recieve(MWWorld *world, MWWorldObjectType type, int value);
}

#endif
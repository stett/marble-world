#include "MWPart.h"
#include "MWMarble.h"
#include "GLGraphicAnimation.h"

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
#include "MWUniversal.h"
#include "MWWorld.h"
#include "MWPath.h"
#include "MWMath.h"
#include "GLDraw.h"
/// TEMP
#include <iostream>
///
using namespace std;

/// 'Tors
MWPart::MWPart() {
    defaults();
}
MWPart::~MWPart() {
}
void MWPart::defaults() {
    x = y = 0.f;
    angle = 0.f;
    r = 0.f;
    path_set_index = -1;
    update = &MWPartDefaults::part_update;
    marble_in = &MWPartDefaults::part_marble_in;
    marble_out = &MWPartDefaults::part_marble_out;
    signal_recieve = &MWPartDefaults::signal_recieve;
    id = -1;
}

/// Methods
void MWPart::draw_back() {
    /// TEMP
    //glColor3f(0.f, 0.f, 0.f);
    ///
    glPushMatrix();
    glTranslatef(x, y, 0.f);
    glRotatef(angle, 0.f, 0.f, 1.f);
        for (int i = 0; i < (int)graphics_back.size(); i ++) graphics_back[i].draw();
    glPopMatrix();
    /// TEMP
    //glColor3f(1.f, 1.f, 1.f);
    ///
}
void MWPart::draw_fore() {
    /// TEMP
    //glColor3f(0.f, 0.f, 0.f);
    ///
    glPushMatrix();
    glTranslatef(x, y, 0.f);
    glRotatef(angle, 0.f, 0.f, 1.f);
        for (int i = 0; i < (int)graphics_fore.size(); i ++) graphics_fore[i].draw();
    glPopMatrix();
    /// TEMP
    //glColor3f(1.f, 1.f, 1.f);
    ///
}
void MWPart::draw_marbles() {
    for (int i = 0; i < (int)marbles.size(); i ++) {
        switch (marbles[i].type) {
        case MWPartMarbleType::PathedMarble:
            marble(i)->draw();
            break;
        case MWPartMarbleType::PathedRobot:
            robot(i)->draw();
            break;
        }
    }
}
void MWPart::update_radius() {
    r = 0.f;
    for (int i = 0; i < (int)path_sets.size(); i ++)
        for (int ii = 0; ii < (int)path_sets[i].paths_local.size(); ii ++)
            r = max(r, path_sets[i].paths_local[ii].r);
    for (int i = 0; i < (int)connection_vertices.size(); i ++) {
        static float dx = connection_vertices[i].vertex_local.x;
        static float dy = connection_vertices[i].vertex_local.y;
        r = max(r, sqrt(dx*dx + dy*dy));
    }
}
void MWPart::globalize_coordinates() {
    glPushMatrix();
    glRotatef(-angle, 0.f, 0.f, 1.f);
    glTranslatef(-x, -y, 0.f);

    // Globalize path vertices
    for (int i = 0; i < (int)path_sets.size(); i ++) {
        path_sets[i].paths_global = path_sets[i].paths_local;
        for (int ii = 0; ii < (int)path_sets[i].paths_global.size(); ii ++)
            path_sets[i].paths_global[ii].globalize_coordinates();
    }

    // Globalize connection vertices
    for (int i = 0; i < (int)connection_vertices.size(); i ++)
        connection_vertices[i].globalize();

    glPopMatrix();
}
void MWPart::connect_by_proximity() {

    // Clear previous connections & then loop through all other parts looking for connectablel vertices
    disconnect();
    int part_index_0 = id;
    for (int part_index_1 = 0; part_index_1 < (int)world->parts.size(); part_index_1 ++) {

        // Skip this part match if they are the same part or if they are not close enough
        if (part_index_0 == part_index_1) continue;
        MWPart *part_0 = this;
        MWPart *part_1 = &world->parts[part_index_1];
        vector<MWPartPathSet> *path_sets_0 = &part_0->path_sets;
        vector<MWPartPathSet> *path_sets_1 = &part_1->path_sets;
        float dx = x - part_1->x;
        float dy = y - part_1->y;
        float max_r = r + part_1->r + 2 * CONNECTION_RADIUS;
        if (dx*dx + dy*dy > max_r*max_r) continue;

        // Loop through every possible vertex pair
        for (int path_set_index_0 = 0; path_set_index_0 < (int)part_0->path_sets.size(); path_set_index_0 ++) {
        for (int path_set_index_1 = 0; path_set_index_1 < (int)part_1->path_sets.size(); path_set_index_1 ++) {
            MWPartPathSet *path_set_0 = &path_sets_0->at(path_set_index_0);
            MWPartPathSet *path_set_1 = &path_sets_1->at(path_set_index_1);
            for (int path_index_0 = 0; path_index_0 < (int)path_set_0->paths_global.size(); path_index_0 ++) {
            for (int path_index_1 = 0; path_index_1 < (int)path_set_1->paths_global.size(); path_index_1 ++) {
                MWPath *path_0 = &path_set_0->paths_global[path_index_0];
                MWPath *path_1 = &path_set_1->paths_global[path_index_1];
                for (int vertex_index_0 = 0; vertex_index_0 < (int)path_0->vertices.size(); vertex_index_0 ++) {
                for (int vertex_index_1 = 0; vertex_index_1 < (int)path_1->vertices.size(); vertex_index_1 ++) {
                    MWPathVertex *vertex_0 = &path_0->vertices[vertex_index_0];
                    MWPathVertex *vertex_1 = &path_1->vertices[vertex_index_1];

                    // If these vertices are close enough together, connect them
                    dx = vertex_0->x - vertex_1->x;
                    dy = vertex_0->y - vertex_1->y;
                    if (dx*dx + dy*dy > CONNECTION_RADIUS*CONNECTION_RADIUS) continue;
                    world->connect_parts(part_index_0, path_set_index_0, path_index_0, vertex_index_0,
                                         part_index_1, path_set_index_1, path_index_1, vertex_index_1,
                                         false);
                }
                }
            }
            }
        }
        }
    }
}
void MWPart::disconnect() {
    for (int path_set_index_0 = 0; path_set_index_0 < (int)path_sets.size(); path_set_index_0 ++) {

        // Remove the connections that other parts have to this path set
        MWPartPathSet *path_set_0 = &path_sets[path_set_index_0];
        for (int part_index_1 = 0; part_index_1 < (int)world->parts.size(); part_index_1 ++) {
            if (part_index_1 == id) continue;
            MWPart *part_1 = &world->parts[part_index_1];
            for (int path_set_index_1 = 0; path_set_index_1 < (int)part_1->path_sets.size(); path_set_index_1 ++) {
                MWPartPathSet *path_set_1 = &part_1->path_sets[path_set_index_1];
                bool there_still_might_be_connections_to_remove = true;
                while (there_still_might_be_connections_to_remove) {
                    there_still_might_be_connections_to_remove = false;
                    for (int connection_index_1 = 0; connection_index_1 < (int)path_set_1->connections.size(); connection_index_1 ++) {
                        MWPartConnection *connection_1 = &path_set_1->connections[connection_index_1];
                        if (connection_1->part_index_other == id) {
                            path_set_1->connections.erase(path_set_1->connections.begin() + connection_index_1);
                            there_still_might_be_connections_to_remove = true;
                        }
                    }
                }
            }
        }

        // Clear this path set's connections
        path_set_0->connections.clear();
    }
}
MWPartConnection *MWPart::get_connection(int path_set_index_, int path_index, int vertex_index, bool return_0_if_currently_invalid) {

    MWPartPathSet *path_set = &path_sets[path_set_index_];
    for (int connection_index = 0; connection_index < (int)path_set->connections.size(); connection_index ++) {
        MWPartConnection *connection = &path_set->connections[connection_index];
        if (connection->path_index[0] == path_index && connection->vertex_index[0] == vertex_index) {
            if (!return_0_if_currently_invalid) return connection;
            MWPart *part_1 = &world->parts[connection->part_index_other];
            if (part_1->path_set_index == connection->path_set_index_other) return connection;
        }
    }
    return 0;
}
path_vec *MWPart::path_set_local() {
    return (path_set_index >= 0 && path_set_index < (int)path_sets.size()) ? &path_sets[path_set_index].paths_local : 0;
}
path_vec *MWPart::path_set_global() {
    return (path_set_index >= 0 && path_set_index < (int)path_sets.size()) ? &path_sets[path_set_index].paths_global : 0;
}
MWMarble *MWPart::marble(int marble_local_index) {
    return &world->marbles[marbles[marble_local_index].index];
}
MWRobot *MWPart::robot(int marble_local_index) {
    return &world->robots[marbles[marble_local_index].index];
}

/// Default part type functions
void MWPartDefaults::part_update(MWPart *part) {
    MWPartDefaults::part_update_graphics(part);
    MWPartDefaults::part_update_marbles(part);
}
void MWPartDefaults::part_update_graphics(MWPart *part) {
    static int i;
    for (i = 0; i < (int)part->graphics_back.size(); i ++)
        part->graphics_back[i].update();
    for (i = 0; i < (int)part->graphics_fore.size(); i ++)
        part->graphics_fore[i].update();
}
void MWPartDefaults::part_update_marbles(MWPart *part) {

    static MWPartMarble *marble_container;
    //static MWMarble *marble;

    static float dx, dy;
    static float ax, ay;
    static float nx, ny;
    static float g_over_d;
    static float v_mag;

    static int i_path;
    static int i_vertex;
    static float path_pos;
    static float path_vel;

    static MWPath *path;
    static MWPathVertex *vertex;
    static MWPathVertex *vertex_1;
    static MWPartConnection *connection;
    static MWPart *part_1;
    static MWPath *path_1;

    // Update the marbles
    for (int i_local_marble = 0; i_local_marble < (int)part->marbles.size(); i_local_marble ++) {

        marble_container = &part->marbles[i_local_marble];

        MWMarble *marble;
        MWRobot *robot;
        float *m_r;
        float *m_x;
        float *m_y;
        float *m_vx;
        float *m_vy;
        float *m_angle;
        float *m_angle_v;
        switch(marble_container->type) {
        case MWPartMarbleType::PathedMarble:
            marble = part->marble(i_local_marble);
            m_r = &marble->r;
            m_x = &marble->x;
            m_y = &marble->y;
            m_vx = &marble->vx;
            m_vy = &marble->vy;
            m_angle = &marble->angle_back;
            m_angle_v = &marble->angle_v;
            break;
        case MWPartMarbleType::PathedRobot:
            robot = part->robot(i_local_marble);
            m_r = &robot->r;
            m_x = &robot->x;
            m_y = &robot->y;
            m_vx = &robot->vx;
            m_vy = &robot->vy;
            m_angle = &robot->angle;
            m_angle_v = &robot->angle_v;
            break;
        }
        
        i_path = marble_container->path_index;
        i_vertex = marble_container->vertex_index;
        path_pos = marble_container->path_position;
        path_vel = marble_container->path_velocity;
        path = &part->path_sets[part->path_set_index].paths_global[i_path];
        /// NOTE: MUST FIX THIS PROBLEM... 
        /// TEMP
        if (i_vertex >= (int)path->vertices.size()) i_vertex = (int)path->vertices.size() - 1;
        ///
        vertex = &path->vertices[i_vertex];
        nx = vertex->nx;
        ny = vertex->ny;

        // Calculate the marble's acceleration vector
        dx = *m_x - WORLD_CENTER_X;
        dy = *m_y - WORLD_CENTER_Y;
        g_over_d = (!(dx == 0 && dy == 0)) ? part->world->gravitation / sqrt(dx*dx + dy*dy) : 0.f;
        ax = -dx * g_over_d;
        ay = -dy * g_over_d;

        // Update the marble's velocity & make it paralel to the current path
        path_vel += MWMath_dot_product(nx, ny, ax, ay);

        // Update the marble's position
        path_pos += path_vel;
        *m_x = vertex->x + nx * path_pos;
        *m_y = vertex->y + ny * path_pos;
        marble_container->path_position = path_pos;
        marble_container->path_velocity = path_vel;

        // Update the marble's angle
        float d_angle = 0.0349066f * (*m_r) * path_vel * (MWMath_dot_product(-ny, nx, dx, dy) < 0.f ? 1.f : -1.f);
        *m_angle += d_angle;

        // Check to see if the marble has gone past the end of this path segment
        if (path_pos > vertex->l) {

            // And if this is the last vertex...
            if (i_vertex >= (int)path->vertices.size() - 1) {
                connection = part->get_connection(part->path_set_index, i_path, i_vertex);

                // If nothing is connected, here, depath the marble
                if (!connection) {
                    *m_vx = nx * path_vel;
                    *m_vy = ny * path_vel;    
                    *m_angle_v = d_angle;
                    part->marble_out(part, i_local_marble);
                    return;
                }

                part_1 = &part->world->parts[connection->part_index_other];
                path_1 = &part_1->path_sets[connection->path_set_index_other].paths_global[connection->path_index[1]];
                marble_container->path_index = connection->path_index[1];

                marble_container->vertex_index = connection->vertex_index[1];
                vertex_1 = &path_1->vertices[marble_container->vertex_index];
                marble_container->path_position = (marble_container->vertex_index == 0) ? path_pos - vertex->l : 2 * vertex_1->l - path_pos;
                marble_container->path_velocity = path_vel * MWMath_dot_product(vertex_1->nx, vertex_1->ny, nx, ny);
                /// TEMP
                // This makes it so that going around corners does not decrease velocity at all... it's non-physical, but it actually looks more realistic this way
                // TODO: This math is kind of messy - there's bound to be a better way of doing it.
                marble_container->path_velocity = abs(path_vel) * marble_container->path_velocity / abs(marble_container->path_velocity);
                ///

                part->marble_in(part_1, *marble_container);
                part->marbles.erase(part->marbles.begin() + i_local_marble);
            } else {
                i_vertex ++;
                marble_container->path_position = path_pos - vertex->l;
                vertex = &path->vertices[i_vertex];
                marble_container->vertex_index = i_vertex;
                ///marble_container->path_velocity *= MWMath_dot_product(nx, ny, vertex->nx, vertex->ny);
            }
        } else if (path_pos < 0.f) {

            // If this is the first vertex
            if (i_vertex <= 0) {
                connection = part->get_connection(part->path_set_index, i_path, i_vertex);

                // If nothing is connected, here, depath the marble
                if (!connection) {
                    *m_vx = nx * path_vel;
                    *m_vy = ny * path_vel;
                    *m_angle_v = d_angle;
                    part->marble_out(part, i_local_marble);
                    return;
                }
                
                part_1 = &part->world->parts[connection->part_index_other];
                path_1 = &part_1->path_sets[connection->path_set_index_other].paths_global[connection->path_index[1]];
                marble_container->path_index = connection->path_index[1];

                marble_container->vertex_index = connection->vertex_index[1];
                vertex_1 = &path_1->vertices[marble_container->vertex_index];
                marble_container->path_position = (marble_container->vertex_index == 0) ? -path_pos : path_pos;
                marble_container->path_velocity = path_vel * MWMath_dot_product(vertex_1->nx, vertex_1->ny, nx, ny);
                /// TEMP
                marble_container->path_velocity = abs(path_vel) * marble_container->path_velocity / abs(marble_container->path_velocity);
                ///

                part->marble_in(part_1, *marble_container);
                part->marbles.erase(part->marbles.begin() + i_local_marble);
            } else {
                i_vertex --;
                vertex = &path->vertices[i_vertex];
                marble_container->vertex_index = i_vertex;
                marble_container->path_position = vertex->l + path_pos;
                ///marble_container->path_velocity *= MWMath_dot_product(nx, ny, vertex->nx, vertex->ny);
            }
        }
    }
}
void MWPartDefaults::part_marble_in(MWPart *part, const MWPartMarble &marble_container) {
    switch (marble_container.type) {
    case (MWPartMarbleType::PathedMarble):
        part->world->marbles[marble_container.index].pathed = true;
        break;
    case (MWPartMarbleType::PathedRobot):
        part->world->robots[marble_container.index].pathed = true;
        break;
    }
    part->marbles.push_back(marble_container);
    return;
}
void MWPartDefaults::part_marble_out(MWPart *part, int marble_local_index) {
    MWPartMarble *marble_container = &part->marbles[marble_local_index];
    switch (marble_container->type) {
    case (MWPartMarbleType::PathedMarble):
        part->world->marbles[marble_container->index].pathed = false;
        break;
    case (MWPartMarbleType::PathedRobot):
        part->world->robots[marble_container->index].pathed = false;
        break;
    }
    part->marbles.erase(part->marbles.begin() + marble_local_index);
    return;
}
void MWPartDefaults::signal_recieve(MWPart *part, MWWorldObjectType type, int value) {
    return;
}
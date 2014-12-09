#include "MWWorld.h"
#include "MWPath.h"
#include "MWPart.h"
#include "GLDraw.h"
#include "MWMath.h"
#include "MWUniversal.h"
#include "MWParts.h"
#include "MWMarbles.h"
#include "MWProps.h"

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

/// 'Tors
MWWorld::MWWorld() {
    defaults();
}
MWWorld::~MWWorld() {
}
void MWWorld::defaults() {
    planet_radius = 2500.f;
    gravitation = 1.f;
    update_atmosphere = &MWWorldDefaults::update_atmosphere;
    draw_back = &MWWorldDefaults::draw_back;
    draw_fore = &MWWorldDefaults::draw_fore;
    signal_recieve = &MWWorldDefaults::signal_recieve;
    lightness_parts = 1.f;
    lightness_props = 0.f;
    view.angle = 0.f;
    view.x = WORLD_CENTER_X - SCREEN_W / 2;
    view.y = WORLD_CENTER_Y - SCREEN_H / 2;
    view.scale = 1.f;
}

/// Methods
void MWWorld::update() {
    update_globalization();
    update_robots();
    update_marbles();
    update_parts();
    update_atmosphere(this);
}
void MWWorld::update_globalization() {

    // Globalize part paths
    for (int i = 0; i < (int)globalize_part_indices.size(); i ++)
        parts[globalize_part_indices[i]].globalize_coordinates();
    globalize_part_indices.clear();

    // Globalize prop vertices
    for (int i = 0; i < (int)globalize_prop_indices.size(); i ++)
        props[globalize_prop_indices[i]].globalize_connection_vertices();
    globalize_prop_indices.clear();
}
void MWWorld::update_part_globalization(int part_index) {
    globalize_part_indices.push_back(part_index);
}
void MWWorld::update_prop_globalization(int prop_index) {
    globalize_prop_indices.push_back(prop_index);
}
void MWWorld::update_robots() {

    for (int i = 0; i < (int)robots.size(); i ++) {
        MWRobot *robot = &robots[i];
        if (robot->pathed) continue;

        // Update the robot's position based on its velocity
        robot->x += robot->vx;
        robot->y += robot->vy;
        robot->angle += robot->angle_v;

        // Check for "enpathment"
        int i_part;
        MWPartMarble marble_container;
        if (enpath(&i_part, &marble_container, MWPartMarbleType::PathedRobot, i, robot->x, robot->y, robot->vx, robot->vy, robot->r)) {
            MWPart *part = &parts[i_part];
            part->marble_in(part, marble_container);
        }

        // Calculate the robot's acceleration vector
        float dx = robot->x - WORLD_CENTER_X;
        float dy = robot->y - WORLD_CENTER_Y;
        float g_over_d = (!(dx == 0 && dy == 0)) ? gravitation / sqrt(dx*dx + dy*dy) : 0.f;
        float ax = -dx * g_over_d;
        float ay = -dy * g_over_d;

        // Update the robot's velocity
        robot->vx += ax;
        robot->vy += ay;

        dx = robot->x + robot->vx - WORLD_CENTER_X;
        dy = robot->y + robot->vy - WORLD_CENTER_Y;
        if (dx*dx + dy*dy <= planet_radius) {
            float v = MWMath_dot_product(robot->vx, robot->vy, -dy, dx);
            //MWMath_normalize(-dy, dx, &
        }

        //
        robot->update();
    }
}
void MWWorld::update_marbles() {
    static float dx, dy, d, d2, d_min, d_inv, g_over_d;
    static float ax, ay;
    static float r0, r1;
    static float m_sum_inv;
    static float m_dif;
    static float vx0, vy0, vx1, vy1;
    static float px, py;
    static float e;
    static float pos;
    for (int i = 0; i < (int)marbles.size(); i ++) {
        MWMarble *marble = &marbles[i];
        if (marble->pathed) continue;

        // Move the marble based on its velocity
        marble->x += marble->vx;
        marble->y += marble->vy;
        marble->angle_back += marble->angle_v;

        // Check for "enpathment"
        int i_part;
        MWPartMarble marble_container;
        if (enpath(&i_part, &marble_container, MWPartMarbleType::PathedMarble, i, marble->x, marble->y, marble->vx, marble->vy, marble->r)) {
            MWPart *part = &parts[i_part];
            part->marble_in(part, marble_container);
            continue;
        }

        // Calculate the marble's acceleration vector
        dx = marble->x - WORLD_CENTER_X;
        dy = marble->y - WORLD_CENTER_Y;
        g_over_d = (!(dx == 0 && dy == 0)) ? gravitation / sqrt(dx*dx + dy*dy) : 0.f;
        ax = -dx * g_over_d;
        ay = -dy * g_over_d;

        // Update the marble's velocity
        marble->vx += ax;
        marble->vy += ay;
    }
}
void MWWorld::update_parts() {
    for (int i = 0; i < (int)parts.size(); i ++)
        parts[i].update(&parts[i]);
}
void MWWorld::update_props() {
    for (int i = 0; i < (int)props.size(); i ++)
        props[i].update(&props[i]);
}
void MWWorld::update_part_indices() {
    for (int i = 0; i < (int)parts.size(); i ++)
        parts[i].id = i;
}
bool MWWorld::enpath(int *part_index, MWPartMarble *marble_container, MWPartMarbleType type, int index, float &marble_x, float &marble_y, float &marble_vx, float &marble_vy, float marble_r) {

    for (int i_part = 0; i_part < (int)parts.size(); i_part ++) {

        // Check distance from this part
        *part_index = i_part;
        MWPart *part = &parts[i_part];
        float dx = marble_x - part->x;
        float dy = marble_y - part->y;
        float d2 = dx*dx + dy*dy;
        float d_min = marble_r + part->r;
        if (d2 > d_min * d_min) continue;

        // Get the current path set for this part
        MWPartPathSet *path_set = &part->path_sets[part->path_set_index];
        if (path_set == 0) continue;

        // ENTRANCE ENPATHMENT
        // Loop through the entrances in this path set
        for (int i_entrance = 0; i_entrance < (int)path_set->entrances.size(); i_entrance ++) {
            MWPartEntrance *entrance = &path_set->entrances[i_entrance];
            MWPath *path = &path_set->paths_global[entrance->path_index];
            MWPathVertex *vert = &path->vertices[entrance->vertex_index];

            // Check for proximity to this vertex
            dx = marble_x - vert->x;
            dy = marble_y - vert->y;
            d2 = dx*dx + dy*dy;
            d_min = entrance->radius;
            if (d2 > d_min * d_min) return false;

            // If it is not quite close enough to enpath, then just accelerate the marble towards the center
            d_min = entrance->inner_radius;
            if (d2 > d_min * d_min) {
                marble_vx -= dx * entrance->suction;
                marble_vy -= dy * entrance->suction;
                if (MWMath_magnitude(marble_vx, marble_vy) > entrance->inner_radius * .5f)
                    MWMath_set_magnitude(&marble_vx, &marble_vy, entrance->inner_radius * .5f);
                return false;
            }

            // It's close enough to the vertex, enpath it
            marble_x = vert->x;
            marble_y = vert->y;
            marble_container->type = type;
            marble_container->index = index;
            marble_container->path_index = entrance->path_index;

            marble_container->vertex_index = entrance->vertex_index;
            vert = &path->vertices[marble_container->vertex_index];
            marble_container->path_position = (marble_container->vertex_index == 0) ? 0 : vert->l;
            marble_container->path_velocity = MWMath_dot_product(vert->nx, vert->ny, marble_vx, marble_vy);

            return true;
        }

        // OPEN-SIDE ENPATHMENT
        // Loop through paths
        for (int i_path_set = 0; i_path_set < (int)part->path_sets.size(); i_path_set ++) {
            path_set = &part->path_sets[i_path_set];
            for (int i_path = 0; i_path < (int)path_set->paths_global.size(); i_path ++) {
                MWPath *path = &path_set->paths_global[i_path];
                for (int i_vertex = 1; i_vertex < (int)path->vertices.size(); i_vertex ++) {
                    MWPathVertex *vert0 = &path->vertices[i_vertex - 1];
                    MWPathVertex *vert1 = &path->vertices[i_vertex];

                    if (vert0->open_side == MWPathVertexOpenSide::NoSide) continue;

                    // Make sure that the marble is on the open side of the path
                    dx = vert1->x - vert0->x;
                    dy = vert1->y - vert0->y;
                    float dx0 = marble_x - vert0->x;
                    float dy0 = marble_y - vert0->y;
                    float nx, ny;
                    MWMath_normalize(-dy, dx, &nx, &ny);
                    float dot_pos = MWMath_dot_product(dx, dy, -dy0, dx0);
                    float dot_vel = MWMath_dot_product(nx, ny, marble_vx, marble_vy);
                    if ((vert0->open_side == MWPathVertexOpenSide::LeftSide && (dot_pos < 0.f || dot_vel < 0.f)) ||
                        (vert0->open_side == MWPathVertexOpenSide::RightSide && (dot_pos > 0.f || dot_vel < 0.f))) {
                        continue;
                    }

                    // Check the distance to the path
                    float px, py;
                    if (MWMath_distance_from_point_to_line_segment(
                        vert0->x, vert0->y,
                        vert1->x, vert1->y,
                        marble_x, marble_y,
                        &px, &py) < marble_r + dot_vel)
                    {
                        // Snap the marble onto the path
                        marble_x = px;
                        marble_y = py;

                        // Make a marble container and push it onto the part's marble list
                        dx = px - vert0->x;
                        dy = py - vert0->y;
                        float pos = sqrt(dx*dx + dy*dy);
                        marble_container->type = type;
                        marble_container->index = index;
                        marble_container->path_index = i_path;
                        marble_container->vertex_index = i_vertex - 1;
                        marble_container->path_position = pos;
                        marble_container->path_velocity = MWMath_dot_product(path->vertices[i_vertex - 1].nx, path->vertices[i_vertex - 1].ny, marble_vx, marble_vy);

                        return true;
                    }
                }
            }
        }
    }
    return false;
}
// Drawing
void MWWorld::draw() {
    static int i;

    // World - background
    draw_back(this);

    // World - planet
    glPushMatrix();
    glTranslatef(WORLD_CENTER_X, WORLD_CENTER_Y, 0.f);
    glBindTexture(GL_TEXTURE_2D, graphic_planet.texture.tex_id);
    glBegin(GL_QUADS);
        glTexCoord2i(0, 0);                 // Top-left
        glVertex2f(-planet_radius, -planet_radius);
        glTexCoord2i(1, 0);                 // Top-right
        glVertex2f(planet_radius, -planet_radius);
        glTexCoord2i(1, 1);                 // Bottom-right
        glVertex2f(planet_radius, planet_radius);
        glTexCoord2i(0, 1);                 // Bottom-left
        glVertex2f(-planet_radius, planet_radius);
    glEnd();
    glPopMatrix();

    // Props
    if (lightness_props != 1) glColor3f(lightness_props, lightness_props, lightness_props);
    for (i = 0; i < (int)props.size(); i ++)
        props[i].draw();
    glColor3f(1.f, 1.f, 1.f);

    // Parts - background
    if (lightness_parts != 1) glColor3f(lightness_parts, lightness_parts, lightness_parts);
    for (i = 0; i < (int)parts.size(); i ++)
        parts[i].draw_back();
    glColor3f(1.f, 1.f, 1.f);

    // Parts - marbles
    for (i = 0; i < (int)parts.size(); i ++)
        parts[i].draw_marbles();

    // Parts - foreground
    if (lightness_parts != 1) glColor3f(lightness_parts, lightness_parts, lightness_parts);
    for (i = 0; i < (int)parts.size(); i ++)
        parts[i].draw_fore();
    glColor3f(1.f, 1.f, 1.f);

    // Marbles - non-pathed
    for (i = 0; i < (int)marbles.size(); i ++)
        if (!marbles[i].pathed) marbles[i].draw();

    // The player's robot
    for (i = 0; i < (int)robots.size(); i ++)
        robots[i].draw();

    // World - foreground
    draw_fore(this);
}
void MWWorld::view_transformations() {
    glTranslatef(SCREEN_W / 2, SCREEN_H / 2, 0.f);
    glScalef(view.scale, view.scale, 1.f);
    glRotatef(view.angle, 0.f, 0.f, 1.f);
    glTranslatef(- view.x - SCREEN_W / 2, - view.y - SCREEN_H / 2, 0.f);
}
void MWWorld::view_inverse_transformations() {
    glTranslatef(view.x + SCREEN_W / 2, view.y + SCREEN_H / 2, 0.f);
    glRotatef(-view.angle, 0.f, 0.f, 1.f);
    glScalef(1.f / view.scale, 1.f / view.scale, 1.f);
    glTranslatef(-SCREEN_W / 2, -SCREEN_H / 2, 0.f);
}
// Part managment
int MWWorld::add_part(void (*part_generator) (MWPart *part), float x, float y, float angle) {
    int part_index = (int)parts.size();
    parts.push_back(make_part(part_generator, x, y, angle));
    globalize_part_indices.push_back(part_index);
    parts[part_index].id = part_index;
    parts[part_index].world = this;
    return part_index;
}
int MWWorld::add_marble(void (*marble_generator) (MWMarble *marble), float x, float y) {
    int marble_index = (int)marbles.size();
    marbles.push_back(make_marble(marble_generator, x, y));
    return marble_index;
}
int MWWorld::add_prop(void (*prop_generator) (MWProp *prop), float x, float y, float angle) {
    int prop_index = (int)props.size();
    props.push_back(make_prop(prop_generator, x, y));
    globalize_prop_indices.push_back(prop_index);
    return prop_index;
}
void MWWorld::remove_part(int part_index) {
    parts[part_index].disconnect();
    parts.erase(parts.begin() + part_index);
    update_part_indices();
    /// TODO:
    /// Test the below mechanism for maintaining link indices
    for (int i = 0; i < (int)parts.size(); i ++)
        for (int ii = 0; ii < (int)parts[i].link_part_indices.size(); ii ++)
            if (parts[i].link_part_indices[ii] >= part_index)
                parts[i].link_part_indices[ii] --;
}
void MWWorld::remove_marble(int marble_index) {
    marbles.erase(marbles.begin() + marble_index);
}
void MWWorld::remove_prop(int prop_index) {
    props.erase(props.begin() + prop_index);
}
void MWWorld::connect_parts(int part_index_0, int path_set_index_0, int path_index_0, int vertex_index_0, 
                            int part_index_1, int path_set_index_1, int path_index_1, int vertex_index_1,
                            bool connect_overlapping_vertices) {
    
    // Get pointers to all the relavant data and make sure all of the indices are valid
    if (part_index_0 >= (int)parts.size() || part_index_1 >= (int)parts.size()) return;
    MWPart *part_0 = &parts[part_index_0];
    MWPart *part_1 = &parts[part_index_1];
    vector<MWPartPathSet> *path_sets_0 = &part_0->path_sets;
    vector<MWPartPathSet> *path_sets_1 = &part_1->path_sets;
    if (path_set_index_0 >= (int)path_sets_0->size() || path_set_index_1 >= (int)path_sets_1->size()) return;
    MWPartPathSet *path_set_0 = &path_sets_0->at(path_set_index_0);
    MWPartPathSet *path_set_1 = &path_sets_1->at(path_set_index_1);
    if (path_index_0 >= (int)path_set_0->paths_local.size() || path_index_1 >= (int)path_set_1->paths_local.size()) return;
    MWPath *path_0 = &path_set_0->paths_local[path_index_0];
    MWPath *path_1 = &path_set_1->paths_local[path_index_1];
    if (vertex_index_0 >= (int)path_0->vertices.size() || vertex_index_1 >= (int)path_1->vertices.size()) return;

    // Create connection objects and give them to the parts' path sets
    MWPartConnection connection_0(part_index_1, path_set_index_1, path_index_0, path_index_1, vertex_index_0, vertex_index_1);
    MWPartConnection connection_1(part_index_0, path_set_index_0, path_index_1, path_index_0, vertex_index_1, vertex_index_0);
    path_set_0->connections.push_back(connection_0);
    path_set_1->connections.push_back(connection_1);

    // Also connect overlapping vertices from all paths in all path sets
    if (!connect_overlapping_vertices) return;
    MWPathVertex *vertex_0 = &path_0->vertices[vertex_index_0];
    MWPathVertex *vertex_1 = &path_1->vertices[vertex_index_1];
    for (int path_set_index_0_ = 0; path_set_index_0_ < (int)part_0->path_sets.size(); path_set_index_0_ ++) {
    for (int path_set_index_1_ = 0; path_set_index_1_ < (int)part_1->path_sets.size(); path_set_index_1_ ++) {
        MWPartPathSet *path_set_0 = &path_sets_0->at(path_set_index_0_);
        MWPartPathSet *path_set_1 = &path_sets_1->at(path_set_index_1_);
        for (int path_index_0_ = 0; path_index_0_ < (int)path_set_0->paths_local.size(); path_index_0_ ++) {
        for (int path_index_1_ = 0; path_index_1_ < (int)path_set_1->paths_local.size(); path_index_1_ ++) {
            MWPath *path_0 = &path_set_0->paths_local[path_index_0_];
            MWPath *path_1 = &path_set_1->paths_local[path_index_1_];
            for (int vertex_index_0_ = 0; vertex_index_0_ < (int)path_0->vertices.size(); vertex_index_0_ ++) {
            for (int vertex_index_1_ = 0; vertex_index_1_ < (int)path_1->vertices.size(); vertex_index_1_ ++) {

                // Skip this pair of vertices if they are the same as the input set.
                if (path_set_index_0_ == path_set_index_0 && path_index_0_ == path_index_0 && vertex_index_0_ == vertex_index_0 &&
                    path_set_index_1_ == path_set_index_1 && path_index_1_ == path_index_1 && vertex_index_1_ == vertex_index_1) 
                    continue;

                // If the vertex set doesn't overlap with the original vertex set, skip it
                MWPathVertex *vertex_0_ = &path_0->vertices[vertex_index_0_];
                MWPathVertex *vertex_1_ = &path_1->vertices[vertex_index_1_];
                if (vertex_0_->x != vertex_0->x || vertex_0_->y != vertex_0->y ||
                    vertex_1_->x != vertex_1->x || vertex_1_->y != vertex_1->y)
                    continue;

                // This vertex set overlaps with the original vertex set - create the connection
                connection_0 = MWPartConnection(part_index_1, path_set_index_1_, path_index_0_, path_index_1_, vertex_index_0_, vertex_index_1_);
                connection_1 = MWPartConnection(part_index_0, path_set_index_0_, path_index_1_, path_index_0_, vertex_index_1_, vertex_index_0_);
                path_set_0->connections.push_back(connection_0);
                path_set_1->connections.push_back(connection_1);
            }
            }
        }
        }
    }
    }
}
// Getters
MWRobot *MWWorld::player() {
    if ((int)robots.size() > 0)
        return &robots[0];
    else
        return 0;
}
// Signal management
void MWWorld::signal_send(MWWorldObjectType type, int reciever_id, int value) {
    switch (type) {
    case Part:
        parts[reciever_id].signal_recieve(&parts[reciever_id], type, value);
        break;
    case World:
        signal_recieve(this, type, value);
        break;
    }
}

/// Default instance functions
void MWWorldDefaults::update_atmosphere(MWWorld *world) {
    int i0, i1;
    for (i0 = 0; i0 < (int)world->atmosphere_back.size(); i0 ++) {
        world->atmosphere_back[i0].angle += (1 + i0) * .01f;
        while (world->atmosphere_back[i0].angle > 360.f) world->atmosphere_back[i0].angle -= 360.f;
    }
    for (i1 = 0; i1 < (int)world->atmosphere_fore.size(); i1 ++) {
        world->atmosphere_fore[i1].angle += (1 + i0 + i1) * .01f;
        while (world->atmosphere_fore[i1].angle > 360.f) world->atmosphere_fore[i1].angle -= 360.f;
    }
}
void MWWorldDefaults::draw_back(MWWorld *world) {

    glBindTexture(GL_TEXTURE_2D, world->atmosphere_sky.texture.tex_id);
    glBegin(GL_QUADS);
        glTexCoord2i(0, 0);         // Top-left vertex (corner)
        glVertex2f(0, 0);
        glTexCoord2i(1, 0);         // Top-right vertex (corner)
        glVertex2f(WORLD_W, 0);
        glTexCoord2i(1, 1);         // Bottom-right vertex (corner)
        glVertex2f(WORLD_W, WORLD_H);
        glTexCoord2i(1, 0);         // Bottom-left vertex (corner)
        glVertex2f(0, WORLD_H);
    glEnd();

    for (int i = 0; i < (int)world->atmosphere_back.size(); i ++)
        world->atmosphere_back[i].graphic.draw(
            WORLD_CENTER_X, WORLD_CENTER_Y,
            world->atmosphere_back[i].angle);
}
void MWWorldDefaults::draw_fore(MWWorld *world) {
    for (int i = 0; i < (int)world->atmosphere_fore.size(); i ++)
        world->atmosphere_fore[i].graphic.draw(
            WORLD_CENTER_X, WORLD_CENTER_Y,
            world->atmosphere_fore[i].angle);
}
/// Default callback functions
void MWWorldDefaults::signal_recieve(MWWorld *world, MWWorldObjectType type, int value) {
    return;
}
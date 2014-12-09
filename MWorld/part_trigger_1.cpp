#include "MWUniversal.h"
#include "MWPart.h"
#include "MWParts.h"
#include "MWMarble.h"
#include "MWMath.h"
#include "MWWorld.h"
#include <vector>
using namespace std;

namespace MW_trigger_1 {
    void update(MWPart *part);
    void part_marble_in(MWPart *part, const MWPartMarble &marble_container);
    vector<int> new_marbles;
};

void PART_TRIGGER_1(MWPart *part) {

    // Instance functions
    part->update = &MW_trigger_1::update;
    part->marble_in = &MW_trigger_1::part_marble_in;

    // Graphics
    {   GLGraphicAnimation anim;
        anim.set_texture(TEX_TRIGGER_1_BACK);
        part->graphics_back.push_back(anim);

        anim.set_textures(TEX_TRIGGER_1_FLAGS);
        anim.frame_vel = 0.f;
        anim.frame_fric = .9f;
        part->graphics_back.push_back(anim);

        anim.set_textures(TEX_TRIGGER_1);
        anim.frame_vel = 0.f;
        anim.frame_fric = .9f;
        part->graphics_fore.push_back(anim);
    }

    // Paths
    {   MWPartPathSet path_set;
        {   MWPath path;
            path.add_vertex(-81, 0);
            path.add_vertex(81, 0);
            path_set.paths_local.push_back(path);
        }
        part->path_sets.push_back(path_set);
    }
    part->path_set_index = 0;
    part->update_radius();
}

void MW_trigger_1::update(MWPart *part) {

    // Update the graphics
    MWPartDefaults::part_update_graphics(part);

    // Do all of the marble math!
    static MWPartMarble *marble_container;
    static MWMarble *marble;

    static float dx, dy;
    static float ax, ay;
    static float nx, ny;
    static float g_over_d;
    static float v_mag;

    static int i_path;
    static int i_vertex;
    static float path_pos0;
    static float path_pos;
    static float path_vel;

    static MWPath *path;
    static MWPathVertex *vertex;
    static MWPathVertex *vertex_1;
    static MWPartConnection *connection;
    static MWPart *part_1;
    static MWPath *path_1;

    // Update the marbles
    for (int i_marble = 0; i_marble < (int)part->marbles.size(); i_marble ++) {
        marble_container = &part->marbles[i_marble];
        int new_marble_index = -1;
        for (int i_new_marbles = 0; i_new_marbles < (int)new_marbles.size(); i_new_marbles ++) {
            if (new_marbles[i_new_marbles] == marble_container->index) {
                new_marble_index = i_new_marbles;
                break;
            }
        }
        marble = part->marble(i_marble);
        i_path = marble_container->path_index;
        i_vertex = marble_container->vertex_index;
        path_pos0 =
        path_pos = marble_container->path_position;
        path_vel = marble_container->path_velocity;
        path = &part->path_sets[part->path_set_index].paths_global[i_path];
        vertex = &path->vertices[i_vertex];
        nx = vertex->nx;
        ny = vertex->ny;

        // Calculate the marble's acceleration vector
        dx = marble->x - WORLD_CENTER_X;
        dy = marble->y - WORLD_CENTER_Y;
        g_over_d = (!(dx == 0 && dy == 0)) ? part->world->gravitation / sqrt(dx*dx + dy*dy) : 0.f;
        ax = -dx * g_over_d;
        ay = -dy * g_over_d;

        // Update the marble's velocity & make it parallel to the current path
        path_vel += MWMath_dot_product(nx, ny, ax, ay);

        // Update the marble's position
        path_pos += path_vel;
        marble->x = vertex->x + nx * path_pos;
        marble->y = vertex->y + ny * path_pos;
        marble_container->path_position = path_pos;
        marble_container->path_velocity = path_vel;

        // Check to see if the marble passed the center point
        float vertex_l2 = path->vertices[0].l * .5f;
        float offset = 20.f;
        if ((new_marble_index > -1 && ((path_pos0 > vertex_l2 && path_vel > 0) || (path_pos0 < vertex_l2 && path_vel < 0))) 
        || (path_pos0 < vertex_l2 && path_pos > vertex_l2) || (path_pos0 > vertex_l2 && path_pos < vertex_l2)) {

            // Send signals to all the linked parts
            for (int i_link = 0; i_link < (int)part->link_part_indices.size(); i_link ++)
                part->world->signal_send(Part, part->link_part_indices[i_link], PART_SIGNAL_TRIGGER);

            // Spin the flags
            part->graphics_back[1].frame_vel = path_vel * .3f;
            part->graphics_fore[0].frame_vel = path_vel * .3f;
        }
        if (new_marble_index > -1) new_marbles.erase(new_marbles.begin() + new_marble_index);

        if (path_pos > vertex->l) {                                         // Check to see if the marble has gone past the end of this path segment
            if (i_vertex >= (int)path->vertices.size() - 1) {               //   And if this is the last vertex
                connection = part->get_connection(part->path_set_index, i_path, i_vertex);
                if (!connection) {
                    part->marble_out(part, i_marble);          //   If nothing is connected, here, depath the marble
                    return;
                }
                
                part_1 = &part->world->parts[connection->part_index_other];
                path_1 = &part_1->path_sets[connection->path_set_index_other].paths_global[connection->path_index[1]];
                marble_container->path_index = connection->path_index[1];

                marble_container->vertex_index = connection->vertex_index[1];
                vertex_1 = &path_1->vertices[marble_container->vertex_index];
                marble_container->path_position = (marble_container->vertex_index == 0) ? path_pos - vertex->l : 2 * vertex_1->l - path_pos;
                marble_container->path_velocity = path_vel * MWMath_dot_product(vertex_1->nx, vertex_1->ny, nx, ny);

                part->marble_in(part_1, *marble_container);
                part->marbles.erase(part->marbles.begin() + i_marble);
            } else {
                i_vertex ++;
                marble_container->path_position = path_pos - vertex->l;
                vertex = &path->vertices[i_vertex];
                marble_container->vertex_index = i_vertex;
                marble_container->path_velocity *= MWMath_dot_product(nx, ny, vertex->nx, vertex->ny);
            }
        } else if (path_pos < 0.f) {
            if (i_vertex <= 0) {                                            //   If this is the first vertex
                connection = part->get_connection(part->path_set_index, i_path, i_vertex);
                if (!connection) {
                    part->marble_out(part, i_marble);          //   If nothing is connected, here, depath the marble
                    return;
                }
                
                part_1 = &part->world->parts[connection->part_index_other];
                path_1 = &part_1->path_sets[connection->path_set_index_other].paths_global[connection->path_index[1]];
                marble_container->path_index = connection->path_index[1];

                marble_container->vertex_index = connection->vertex_index[1];
                vertex_1 = &path_1->vertices[marble_container->vertex_index];
                marble_container->path_position = (marble_container->vertex_index == 0) ? -path_pos : path_pos;
                marble_container->path_velocity = path_vel * MWMath_dot_product(vertex_1->nx, vertex_1->ny, nx, ny);

                part->marble_in(part_1, *marble_container);
                part->marbles.erase(part->marbles.begin() + i_marble);
            } else {
                i_vertex --;
                vertex = &path->vertices[i_vertex];
                marble_container->vertex_index = i_vertex;
                marble_container->path_position = vertex->l + path_pos;
                marble_container->path_velocity *= MWMath_dot_product(nx, ny, vertex->nx, vertex->ny);
            }
        }
    }
}

void MW_trigger_1::part_marble_in(MWPart *part, const MWPartMarble &marble_container) {
    part->world->marbles[marble_container.index].pathed = true;
    part->marbles.push_back(marble_container);
    new_marbles.push_back(marble_container.index);
    return;
}
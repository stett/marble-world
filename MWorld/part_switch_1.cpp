#include "MWUniversal.h"
#include "MWPart.h"
#include "MWParts.h"

namespace MW_switch_1 {
    void signal_recieve(MWPart *part, MWWorldObjectType type, int value);
};

void PART_SWITCH_1(MWPart *part) {

    // Instance functions
    part->signal_recieve = &MW_switch_1::signal_recieve;

    // Graphics
    {   GLGraphicAnimation anim;
        anim.set_texture(TEX_SWITCH_1_BACK);
        part->graphics_back.push_back(anim);
        
        anim.set_textures(TEX_SWITCH_1_FLAGS);
        anim.target(0);
        part->graphics_back.push_back(anim);
        
        anim.set_textures(TEX_SWITCH_1);
        anim.target(0);
        part->graphics_fore.push_back(anim);
    }

    // Paths
    /* [1]          (0.2)---(0.3)
                    /
        (0.0)---(0.1)

                    (1.0)---(1.1)
    */
    /* [2]          (1.0)---(1.1)

        (0.0)---(0.1)
                    \
                    (0.2)---(0.3)
    */
    {   MWPartPathSet path_set;
        {   MWPath path;
            path.add_vertex(-129, 0);
            path.add_vertex(-48, 0);
            path.add_vertex(48, -56);
            path.add_vertex(129, -56);
            path_set.paths_local.push_back(path);
        } { MWPath path;
            path.add_vertex(48, 56);
            path.add_vertex(129, 56);
            path_set.paths_local.push_back(path);
        }
        part->path_sets.push_back(path_set);
    } { MWPartPathSet path_set;
        {   MWPath path;
            path.add_vertex(-129, 0);
            path.add_vertex(-48, 0);
            path.add_vertex(48, 56);
            path.add_vertex(129, 56);
            path_set.paths_local.push_back(path);
        } { MWPath path;
            path.add_vertex(48, -56);
            path.add_vertex(129, -56);
            path_set.paths_local.push_back(path);
        }
        part->path_sets.push_back(path_set);
    }
    part->path_set_index = 0;
    part->update_radius();
}

void MW_switch_1::signal_recieve(MWPart *part, MWWorldObjectType type, int value) {
    MWPartMarble *marble;

    if (part->path_set_index == 0) {
        part->path_set_index = 1;
        part->graphics_back[1].target(part->graphics_back[1].frame_count - 1.f);
        part->graphics_fore[0].target(part->graphics_fore[0].frame_count - 1.f);

        for (int i = 0; i < (int)part->marbles.size(); i ++) {
            marble = &part->marbles[i];
            if (marble->path_index == 0) {
                if (marble->vertex_index == 2) {
                    marble->path_index = 1;
                    marble->vertex_index = 0;
                }
            } else if (marble->path_index == 1) {
                if (marble->vertex_index == 0) {
                    marble->path_index = 0;
                    marble->vertex_index = 2;
                }
            }
        }
    } else {
        part->path_set_index = 0;
        part->graphics_back[1].target(0);
        part->graphics_fore[0].target(0);

        for (int i = 0; i < (int)part->marbles.size(); i ++) {
            marble = &part->marbles[i];
            if (marble->path_index == 0) {
                if (marble->vertex_index == 2) {
                    marble->path_index = 1;
                    marble->vertex_index = 0;
                }
            } else if (marble->path_index == 1) {
                if (marble->vertex_index == 0) {
                    marble->path_index = 0;
                    marble->vertex_index = 2;
                }
            }
        }
    }
    return;
}
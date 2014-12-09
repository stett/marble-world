#include "MWPart.h"
#include "MWParts.h"
#include "GLTextures.h"

void PART_TRACK_ENTRANCE_END_1(MWPart *part) {

    // Graphics
    {   GLGraphicAnimation anim;
        anim.set_texture(TEX_TRACK_ENTRANCE_END_1_BACK);
        part->graphics_back.push_back(anim);

        anim.set_texture(TEX_TRACK_ENTRANCE_END_1);
        part->graphics_fore.push_back(anim);
    }

    // Paths
    {   MWPartPathSet path_set;
        {   MWPath path;
            path.add_vertex(-92, 0);
            int entrance_index = path.add_vertex(35, 0);
            path_set.paths_local.push_back(path);
            path_set.entrances.push_back(MWPartEntrance(1));
        }
        part->path_sets.push_back(path_set);
    }
    part->path_set_index = 0;
    
    // Connectors
    {   MWPropConnectionVertexSet vertex_set;
        vertex_set.vertex_local.x = 0;
        vertex_set.vertex_local.y = 0;
        part->connection_vertices.push_back(vertex_set);
    }

    // Update the part's radius based on connection and path vertices
    part->update_radius();
}
#include "MWProp.h"
#include "MWProps.h"

void PROP_SUPPORT_1_TOP(MWProp *prop) {

    // Graphics
    GLGraphicAnimation anim;
    anim.set_texture(TEX_SUPPORT_1_TOP);
    prop->graphics.push_back(anim);

    // Connection vertices
    MWPropConnectionVertexSet vertex_set;
    vertex_set.vertex_local.x = 0.f;
    vertex_set.vertex_local.y = -110.f;
    prop->connection_vertices.push_back(vertex_set);
    vertex_set.vertex_local.x = 0.f;
    vertex_set.vertex_local.y = 115.f;
    prop->connection_vertices.push_back(vertex_set);
    prop->update_radius();
}
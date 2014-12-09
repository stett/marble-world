#include "MWMarble.h"
#include "MWMarbles.h"

void MARBLE_STEEL(MWMarble *marble) {

    // Graphics
    marble->graphic_fore.set_texture(TEX_MARBLE_1);
    marble->graphic_back.set_texture(TEX_MARBLE_1_BACK);

    // Physical properties
    marble->m = 1.f;
    marble->r = 45.f;
    marble->e = .99f;
}
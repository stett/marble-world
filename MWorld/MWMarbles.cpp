#include "MWMarble.h"
#include "MWMarbles.h"

MWMarble make_marble(void (*marble_generator) (MWMarble *marble), float x, float y) {
    MWMarble marble;
    marble_generator(&marble);
    marble.x = x;
    marble.y = y;
    return marble;
}
#include "MWProp.h"
#include "MWProps.h"

MWProp make_prop(void (*prop_generator) (MWProp *prop), float x, float y, float angle) {
    MWProp prop;
    prop_generator(&prop);
    prop.x = x;
    prop.y = y;
    prop.angle = angle;
    return prop;
}
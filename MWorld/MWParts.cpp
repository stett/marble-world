#include "MWPart.h"
#include "MWParts.h"

MWPart make_part(void (*part_generator) (MWPart *part), float x, float y, float angle) {
    MWPart part;
    part_generator(&part);
    part.x = x;
    part.y = y;
    part.angle = angle;
    return part;
}
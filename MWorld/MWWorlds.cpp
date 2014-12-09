#include "MWWorld.h"
#include "MWWorlds.h"

MWWorld make_world(void (*world_generator) (MWWorld *world)) {
    MWWorld world;
    world_generator(&world);
    return world;
}
#ifndef _MWWORLDS_H_
#define _MWWORLDS_H_

class MWWorld;

// Part presets
extern void WORLD_PLANET_1(MWWorld *world);

// Function to generate a particular part
extern MWWorld make_world(void (*world_generator) (MWWorld *world));

#endif
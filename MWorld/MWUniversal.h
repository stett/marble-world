#ifndef _MWUNIVERSAL_H_
#define _MWUNIVERSAL_H_

#include <vector>
using namespace std;

#define WORLD_W (300.f * 64.f)
#define WORLD_H (300.f * 64.f)
#define WORLD_CENTER_X (WORLD_W * .5f)
#define WORLD_CENTER_Y (WORLD_H * .5f)

#define DT 20
#define SCREEN_W 20*64
#define SCREEN_H 12*64

#define CONNECTION_RADIUS 20.f

#define PART_SIGNAL_TRIGGER 0

enum MWWorldObjectType { World, Marble, Part, Prop};

#endif
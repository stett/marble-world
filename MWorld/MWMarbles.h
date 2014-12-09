#ifndef _MWMARBLES_H_
#define _MWMARBLES_H_

class MWMarble;

// Part presets
extern void MARBLE_STEEL(MWMarble *marble);

// Function to generate a particular part
extern MWMarble make_marble(void (*marble_generator) (MWMarble *marble), float x = 0.f, float y = 0.f);

#endif
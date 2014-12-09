#ifndef _MWPROPS_H_
#define _MWPROPS_H_

class MWProp;

// Part presets
extern void PROP_SUPPORT_1_LONG(MWProp *prop);
extern void PROP_SUPPORT_1_SHORT(MWProp *prop);
extern void PROP_SUPPORT_1_TOP(MWProp *prop);

// Function to generate a particular part
extern MWProp make_prop(void (*prop_generator) (MWProp *prop), float x = 0.f, float y = 0.f, float angle = 0.f);

#endif
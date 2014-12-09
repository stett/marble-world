#ifndef _MWPARTS_H_
#define _MWPARTS_H_

class MWPart;

// Part presets
extern void PART_TRACK_1(MWPart *part);
extern void PART_TRACK_ENTRANCE_END_1(MWPart *part);
extern void PART_TRACK_CURVE45_1(MWPart *part);
extern void PART_TRACK_LOOP_SMALL_QUARTER_1(MWPart *part);
extern void PART_TRACK_LOOP_MEDIUM_EIGHTH_1(MWPart *part);
extern void PART_SWITCH_1(MWPart *part);
extern void PART_TRIGGER_1(MWPart *part);

// Function to generate a particular part
extern MWPart make_part(void (*part_generator) (MWPart *part), float x = 0.f, float y = 0.f, float angle = 0.f);

#endif
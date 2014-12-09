#ifndef _MWMATH_H_
#define _MWMATH_H_

extern float MWMath_distance_from_point_to_line_segment(float ax, float ay, float bx, float by, float cx, float cy, float *px = 0, float *py = 0);
extern float MWMath_distance_between_two_line_segments(float ax, float ay, float bx, float by, float cx, float cy, float dx, float dy, float *p0x = 0, float *p0y = 0, float *p1x = 0, float *p1y = 0);
extern float MWMath_magnitude(float x, float y);
extern void MWMath_set_magnitude(float *x, float *y, float magnitude);
extern void MWMath_normalize(float ax, float ay, float *nx, float *ny);
extern float MWMath_dot_product(float ax, float ay, float bx, float by);

#endif
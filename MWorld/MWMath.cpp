#include <math.h>
#include "MWMath.h"

float MWMath_distance_from_point_to_line_segment(float ax, float ay, float bx, float by, float cx, float cy, float *px, float *py) {

    float r_numerator = (cx-ax)*(bx-ax) + (cy-ay)*(by-ay);
    float r_denomenator = (bx-ax)*(bx-ax) + (by-ay)*(by-ay);
    float r = r_numerator / r_denomenator;

    if ((r >= 0) && (r <= 1)) {
        if (px) *px = ax + r*(bx-ax);
        if (py) *py = ay + r*(by-ay);

        float s = ((ay-cy)*(bx-ax)-(ax-cx)*(by-ay) ) / r_denomenator;
        return fabs(s)*sqrt(r_denomenator);
    } else {
        float dist1 = (cx-ax)*(cx-ax) + (cy-ay)*(cy-ay);
        float dist2 = (cx-bx)*(cx-bx) + (cy-by)*(cy-by);

        if (dist1 < dist2) {
            if (px) *px = ax;
            if (px) *py = ay;
            return sqrt(dist1);
        } else {
            if (px) *px = bx;
            if (py) *py = by;
            return sqrt(dist2);
        }
    }
}
float MWMath_distance_between_two_line_segments(float ax, float ay, float bx, float by, float cx, float cy, float dx, float dy, float *p0x, float *p0y, float *p1x, float *p1y) {
    float t_denominator = ax*(cy-dy) + ay*(dx-cx) + bx*(dy-cy) + by*(cx-dx);

    // Check for parallel lines
    if (t_denominator < 0.0001f) {
        if (p0x) *p0x = ax;
        if (p0y) *p0y = ay;
        if (p1x) *p1x = cx;
        if (p1y) *p1y = cy;
        float xx = cx - ax;
        float yy = cy - ay;
        return sqrt(xx*xx + yy*yy);
    }

    float t_denominator_inv = 1.f / t_denominator;
    float t_common = ax*(cy-dy) + ay*(dx-cx);
    float t0 = t_denominator_inv * (t_common + cx*dy - cy*dx);
    float t1 = t_denominator_inv * (t_common + by*cx - bx*cy);

    float P0x, P0y, P1x, P1y;
    /*if (t0 <= 0.f) {
        P0x = ax;
        P0y = ay;
    } else if (t0 >= 1.f) {
        P0x = bx;
        P0y = by;
    } else {*/
        P0x = ax + t0*(bx-ax);
        P0y = ay + t0*(by-ay);
    /*}

    if (t1 <= 0.f) {
        P1x = cx;
        P1y = cy;
    } else if (t0 >= 1.f) {
        P1x = dx;
        P1y = dy;
    } else {*/
        P1x = cx + t1*(dx-cx);
        P1y = cy + t1*(dy-cy);
    //}

    if (p0x) *p0x = P0x;
    if (p0y) *p0y = P0y;
    if (p1x) *p1x = P1x;
    if (p1y) *p1y = P1y;

    float xx = P1x-P0x;
    float yy = P1y-P0y;
    return sqrt(xx*xx + yy*yy);
}
float MWMath_magnitude(float x, float y) {
    return sqrt(x*x + y*y);
}
void MWMath_set_magnitude(float *x, float *y, float magnitude) {
    MWMath_normalize(*x, *y, x, y);
    *x *= magnitude;
    *y *= magnitude;
}
void MWMath_normalize(float ax, float ay, float *nx, float *ny) {
    float d = ax*ax + ay*ay;
    if (d <= 0.f) return;
    float d_inv = 1.f / sqrt(d);
    *nx = ax * d_inv;
    *ny = ay * d_inv;
}
float MWMath_dot_product(float ax, float ay, float bx, float by) {
    return ax * bx + ay * by;
}
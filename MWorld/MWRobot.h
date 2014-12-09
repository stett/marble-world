#ifndef _MWROBOT_H_
#define _MWROBOT_H_

#include "GLGraphicAnimation.h"

enum MWRobotState { Rolling, Extending, Walking, Opening };
enum MWRobotFacing { FacingLeft, FacingRight };

class MWRobot {

    /// Members
public:
    MWRobotState state;
    MWRobotFacing facing;
    bool pathed;
    float x, y;
    float vx, vy;
    float angle;
    float angle_v;
    float r;
    GLGraphicAnimation graphic_fill;
    GLGraphicAnimation graphic_ball;
    GLGraphicAnimation graphic_extend;
    GLGraphicAnimation graphic_walk;
    GLGraphicAnimation graphic_open;

    /// 'TORs
public:
    MWRobot();
    ~MWRobot();
    void defaults();

    /// Methods
public:
    void update();
    void draw();
};

#endif
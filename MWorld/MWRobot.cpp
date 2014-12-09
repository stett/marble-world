#include "MWRobot.h"
#include "MWUniversal.h"
#include "GLTextures.h"

/// 'TORs
MWRobot::MWRobot() {
    defaults();
}
MWRobot::~MWRobot() {}
void MWRobot::defaults() {

    state = Rolling;
    facing = FacingRight;
    pathed = false;
    x = WORLD_CENTER_X;
    y = WORLD_CENTER_Y;
    vx = vy = 0.f;
    angle = 0.f;
    angle_v = 0.f;
    r = 120.f;

    /// TEMP
    graphic_fill.set_texture(TEX_ROBOT_1_BALL_BACK);
    graphic_ball.set_texture(TEX_ROBOT_1_BALL);
    graphic_extend.set_textures(TEX_ROBOT_1_EXTEND);
    graphic_walk.set_textures(TEX_ROBOT_1_WALK);
    //graphic_open.set_textures(TEX_ROBOT_1_OPEN);
    ///
}

/// Methods
void MWRobot::update() {

    //x += vx;
    //y += vy;

    float d_angle = (270.f - 57.2958f * atan2(y - WORLD_CENTER_Y, x - WORLD_CENTER_X)) - angle;
    angle += d_angle;
    while (angle > 360.f) angle -= 360.f;
    while (angle < 0.f) angle += 360.f;

    angle = (90 +  57.2958f * atan2(y - WORLD_CENTER_Y, x - WORLD_CENTER_X));
}
void MWRobot::draw() {

    graphic_fill.draw(x, y);

    switch (state) {
    case Rolling:
        graphic_ball.draw(x, y, angle);
    }
}
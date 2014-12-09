#include "GLGraphicAnimation.h"
#include "GLGraphicList.h"

/// 'Tors
GLGraphicAnimation::GLGraphicAnimation() { defaults(); }
GLGraphicAnimation::GLGraphicAnimation(const GLGraphicAnimation &animation) {
    defaults();
    copy(animation);
}
GLGraphicAnimation::GLGraphicAnimation(const GLGraphicList &graphics_) {
    defaults();
    graphics = graphics;
    frame_count = (int)graphics.textures.size();
}
GLGraphicAnimation::GLGraphicAnimation(const vector<tex> &textures_) {
    defaults();
    graphics.textures = textures_;
    frame_count = (int)graphics.textures.size();
}
GLGraphicAnimation::~GLGraphicAnimation() {
}
void GLGraphicAnimation::defaults() {
    frame = 0.f;
    frame_vel = 1.f;
    frame_fric = 0.f;
    frame_target = 0;
    frame_count = 0;
    mode = CYCLE;
}

/// Methods
void GLGraphicAnimation::set_textures(const tex_v &textures_) {
    graphics.set_textures(textures_);
    frame_count = (int)graphics.textures.size();
}
void GLGraphicAnimation::set_texture(const tex &texture_) {
    graphics.set_texture(texture_);
    frame_count = 1;
}
void GLGraphicAnimation::update() {
    if (frame_count <= 1 || frame_vel == 0.f) return;
    if (mode == CYCLE) {
        frame += frame_vel;
        if (frame_fric != 0.f) frame_vel *= frame_fric;
    } else if (mode == TARGET) {
        if (frame == frame_target) return;
        if (frame < frame_target) {
            frame += abs(frame_vel);
            if (frame > (float)frame_target) frame = (float)frame_target;
        } else {
            frame -= abs(frame_vel);
            if (frame < (float)frame_target) frame = (float)frame_target;
        }
    }
    while (frame >= frame_count) frame -= frame_count;
    while (frame < 0) frame += frame_count;
}
void GLGraphicAnimation::cycle(float frame_vel_) {
    mode = CYCLE;
    frame_vel = frame_vel_;
}
void GLGraphicAnimation::target(float frame_target_) {
    mode = TARGET;
    frame_target = (int)frame_target_;
    while (frame_target >= frame_count) frame_target -= frame_count;
    while (frame_target < 0) frame_target += frame_count;
}
void GLGraphicAnimation::draw() {
    graphics.draw((int)frame);
}
void GLGraphicAnimation::draw(float x, float y) {
    graphics.draw((int)frame, x, y);
}
void GLGraphicAnimation::draw(float x, float y, float angle) {
    graphics.draw((int)frame, x, y, angle);
}
void GLGraphicAnimation::draw(float x, float y, float angle, float scale) {
    graphics.draw((int)frame, x, y, angle, scale);
}
void GLGraphicAnimation::copy(const GLGraphicAnimation &animation) {
    graphics = animation.graphics;
    frame = animation.frame;
    frame_vel = animation.frame_vel;
    frame_fric = animation.frame_fric;
    frame_target = animation.frame_target;
    frame_count = animation.frame_count;
    mode = animation.mode;
}

/// Operators
GLGraphicAnimation& GLGraphicAnimation::operator=(const GLGraphicAnimation &rhs) {
    if (this != &rhs) copy(rhs);
    return *this;
}
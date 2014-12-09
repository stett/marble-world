#ifndef _GLGRAPHICANIMATION_H_
#define _GLGRAPHICANIMATION_H_

#include "GLTextures.h"
#include "GLGraphicList.h"

enum GLGraphicAnimationMode { TARGET, CYCLE };

class GLGraphicAnimation {

    /// Members
private:
    GLGraphicList graphics;
public:
    float frame;
    float frame_vel;
    float frame_fric;
    int frame_target;
    int frame_count;
    GLGraphicAnimationMode mode;

    /// 'Tors
public:
    GLGraphicAnimation();
    GLGraphicAnimation(const GLGraphicAnimation &animation);
    GLGraphicAnimation(const GLGraphicList &graphics_);
    GLGraphicAnimation(const vector<tex> &textures_);
    ~GLGraphicAnimation();
    void defaults();

    /// Methods
public:
    void set_textures(const tex_v &textures_);
    void set_texture(const tex &texture_);
    void update();
    void cycle(float frame_vel_ = 1.f);
    void target(float frame_target_ = 0);
    void draw();
    void draw(float x, float y);
    void draw(float x, float y, float angle);
    void draw(float x, float y, float angle, float scale);
    void copy(const GLGraphicAnimation &animation);

    /// Operators
public:
    GLGraphicAnimation& operator=(const GLGraphicAnimation &rhs);
};

#endif
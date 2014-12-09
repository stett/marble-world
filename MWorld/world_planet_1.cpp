#include "MWWorld.h"
#include "MWWorlds.h"

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

namespace MW_planet_1 {
    void draw_back(MWWorld *world);
    void draw_fore(MWWorld *world);
};

void WORLD_PLANET_1(MWWorld *world) {

    // Graphics
    world->graphic_planet.set_texture(TEX_PLANET_0);

    // Atmosphere
    world->atmosphere_sky.set_texture(TEX_BACKGROUND_0);
    world->atmosphere_back.push_back(MWWorldAtmosphere(TEX_CLOUD_0));
    world->atmosphere_fore.push_back(MWWorldAtmosphere(TEX_CLOUD_1));

    /// TEMP
    world->lightness_parts = 0.f;
    ///

    // Physical properties
    world->gravitation = 1.f;

    // Instance functions
    world->draw_back = &MW_planet_1::draw_back;
    world->draw_fore = &MW_planet_1::draw_fore;
}

void MW_planet_1::draw_back(MWWorld *world) {

    glBindTexture(GL_TEXTURE_2D, world->atmosphere_sky.texture.tex_id);
    glBegin(GL_QUADS);
        glTexCoord2i(0, 0);         // Top-left vertex (corner)
        glVertex2f(0, 0);
        glTexCoord2i(1, 0);         // Top-right vertex (corner)
        glVertex2f(WORLD_W, 0);
        glTexCoord2i(1, 1);         // Bottom-right vertex (corner)
        glVertex2f(WORLD_W, WORLD_H);
        glTexCoord2i(1, 0);         // Bottom-left vertex (corner)
        glVertex2f(0, WORLD_H);
    glEnd();

    static float dx = world->atmosphere_fore[0].graphic.texture.w;
    static float dy = world->atmosphere_fore[0].graphic.texture.h;
    static float scale = 2.f * WORLD_W / sqrt(dx*dx + dy*dy);
    for (int i = 0; i < (int)world->atmosphere_back.size(); i ++)
        world->atmosphere_back[i].graphic.draw(
            WORLD_CENTER_X, WORLD_CENTER_Y,
            world->atmosphere_back[i].angle, scale);
}

void MW_planet_1::draw_fore(MWWorld *world) {
    static float dx = world->atmosphere_fore[0].graphic.texture.w;
    static float dy = world->atmosphere_fore[0].graphic.texture.h;
    static float scale = 2.f * WORLD_W / sqrt(dx*dx + dy*dy);
    for (int i = 0; i < (int)world->atmosphere_fore.size(); i ++)
        world->atmosphere_fore[i].graphic.draw(
            WORLD_CENTER_X, WORLD_CENTER_Y,
            world->atmosphere_fore[i].angle, scale);
}
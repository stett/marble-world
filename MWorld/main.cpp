#include "MWEditor.h"

int main(int argc, char *argv[]) {
    MWEditor editor;
    editor.run();
    return 0;
}


/*
#include <SDL.h>
#include <SDL_gfxBlitFunc.h>
#include <SDL_gfxPrimitives.h>
#include <math.h>
#include <vector>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

#include "SOWV.h"

#include "GLDraw.h"
#include "GLTextures.h"
#include "GLGraphicAnimation.h"

#include "MWUniversal.h"
#include "MWWorld.h"
#include "MWMarble.h"
#include "MWPart.h"
#include "MWParts.h"
#include "MWMath.h"

View    view;
bool    running;
int     bpp;
int     mouse_x_screen;
int     mouse_y_screen;
float   mouse_x;
float   mouse_y;
MWWorld world;

float ax, ay, bx, by, cx, cy, dx, dy, p0x, p0y, p1x, p1y;

void setup_world();
void setup_sdl();
void setup_opengl();
void run();
void process_events();
void update_mouse();
void draw();
void render_map();
void quit(int code = 0);

int main(int argc, char *argv[]) {
    setup_sdl();
    setup_opengl();
    load_all_textures();
    setup_world();
    run();
    quit();
    return 0;
}

void setup_world() {
    

    world.graphic_planet.set_texture(TEX_PLANET_0);
    MWWorldAtmosphere atm_back;
    MWWorldAtmosphere atm_fore0;
    MWWorldAtmosphere atm_fore1;
    atm_back.graphic.set_texture(TEX_BACKGROUND_0);
    atm_fore0.graphic.set_texture(TEX_CLOUD_0);
    atm_fore1.graphic.set_texture(TEX_CLOUD_1);
    world.atmosphere_back.push_back(atm_back);
    world.atmosphere_fore.push_back(atm_fore0);
    world.atmosphere_fore.push_back(atm_fore1);

    MWMarble marble;
    marble.graphic.set_texture(TEX_MARBLE_0);

    marble.x = WORLD_CENTER_X;
    marble.y = WORLD_CENTER_Y - 600;
    world.marbles.push_back(marble);

    world.parts.push_back(make_part(WORLD_CENTER_X, WORLD_CENTER_Y, PART_TRACK_1));
    world.parts.push_back(make_part(WORLD_CENTER_X, WORLD_CENTER_Y - 200, PART_SWITCH_1));
    MW::path_globalization_parts.push_back(&world.parts[0]);
    MW::path_globalization_parts.push_back(&world.parts[1]);
}

void setup_sdl() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) quit();            // Try to initialize sdl subsystems
    const SDL_VideoInfo* info = SDL_GetVideoInfo();     // Get the machine's video information
    bpp = info->vfmt->BitsPerPixel;

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    if(SDL_SetVideoMode(SCREEN_W, SCREEN_H, bpp, SDL_OPENGL) == 0) quit();
}

void setup_opengl() {
    glClearColor(.5f, .5f, .5f, 1.f);
    glViewport(0, 0, SCREEN_W, SCREEN_H);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, SCREEN_W, SCREEN_H, 0.0f, -1.0f, 1.0f);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void run() {
    int dt;
    Uint32 t0;
    Uint32 t1 = SDL_GetTicks() + DT;
    running = true;

    while (running) {
        process_events();
        world.update();
        draw();

        // Delay
        t0 = SDL_GetTicks();
        dt = t1 - t0;
        if (dt < 0) dt = 0;
        SDL_Delay(dt);
        t1 += DT;
    }
}

void draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // View transformations
    glTranslatef(SCREEN_W / 2, SCREEN_H / 2, 0.f);
    glScalef(view.scale, view.scale, 1.f);
    glRotatef(view.angle, 0.f, 0.f, 1.f);
    glTranslatef(- view.x - SCREEN_W / 2, - view.y - SCREEN_H / 2, 0.f);

    // Get the current mouse position in world coordinates
    update_mouse();

    // Draw the world
    world.draw();

    // Draw a marker at the mouse
    GLDraw_circle(mouse_x, mouse_y, 30.f, 3.f, 1.f, 1.f, 1.f);
    //world.marbles[0].draw(mouse_x+30, mouse_y+30);

    // Draw a border around the world
    GLDraw_line(0, 0, WORLD_W, 0, 30);
    GLDraw_line(WORLD_W, 0, WORLD_W, WORLD_H, 30);
    GLDraw_line(WORLD_W, WORLD_H, 0, WORLD_H, 30);
    GLDraw_line(0, WORLD_H, 0, 0, 30);

    // Blit to the screen
    SDL_GL_SwapBuffers();
}

void handle_key_down(SDL_keysym *keysym) {
    float dx = 256.f;
    float da = 15.f;
    float ds = 1.3f;
    switch(keysym->sym) {
    case SDLK_UP:
        view.y -= dx;
        break;
    case SDLK_DOWN:
        view.y += dx;
        break;
    case SDLK_LEFT:
        view.x -= dx;
        break;
    case SDLK_RIGHT:
        view.x += dx;
        break;
    case SDLK_EQUALS:
        view.angle -= da;
        break;
    case SDLK_MINUS:
        view.angle += da;
        break;
    case SDLK_z:
        view.scale /= ds;
        break;
    case SDLK_x:
        view.scale *= ds;
        break;
    case SDLK_ESCAPE:
        running = false;
        break;
    default:
        break;
    }
}

void handle_mouse_down(SDL_MouseButtonEvent *button) {
    switch(button->button) {
    case SDL_BUTTON_LEFT:
        break;
    }
}

void handle_mouse_motion(SDL_MouseMotionEvent *motion) {
    mouse_x_screen = motion->x;
    mouse_y_screen = motion->y;
}

void process_events() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_KEYDOWN:
            handle_key_down(&event.key.keysym);
            break;
        case SDL_MOUSEBUTTONDOWN:
            handle_mouse_down(&event.button);
            break;
        case SDL_MOUSEMOTION:
            handle_mouse_motion(&event.motion);
            break;
        case SDL_QUIT:
            running = false;
            break;
        }
    }
}

void update_mouse() {

    // Get the mouse world coordinates
    static GLint viewport[4];
    static GLdouble modelview[16], projection[16];
    static GLfloat wx, wy;

    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);

    wx = (GLfloat)mouse_x_screen;
    wy = (GLfloat)(viewport[3] - mouse_y_screen);

    //glReadPixels(mouse_x_screen, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &wz);
    static double x, y, z;
    gluUnProject(wx, wy, 0, modelview, projection, viewport, &x, &y, &z);
    mouse_x = (float)x;
    mouse_y = (float)y;
}

void quit(int code) {
    free_all_textures();
    SDL_Quit();
    exit(code);
}
*/
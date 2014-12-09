#ifndef _SOWV_H_
#define _SOWV_H_

#include <SDL.h>

/* SDL interprets each pixel as a 32-bit number, so our masks must depend
    on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    static Uint32 rmask = 0xff000000;
    static Uint32 gmask = 0x00ff0000;
    static Uint32 bmask = 0x0000ff00;
    static Uint32 amask = 0x000000ff;
#else
    static Uint32 rmask = 0x000000ff;
    static Uint32 gmask = 0x0000ff00;
    static Uint32 bmask = 0x00ff0000;
    static Uint32 amask = 0xff000000;
#endif

struct World {
    float w, h;
    unsigned int gl_texture;
};

struct View {
    float x, y;
    float angle;
    float scale;
};

struct Map {
    float scale;
    SDL_Surface *surface;
    unsigned int gl_texture;
};

struct Object {
    unsigned int gl_texture;
    float x, y;
    float w, h;
    float angle;
};

extern bool load_texture(char* file, unsigned int *gl_texture, int *w=0, int *h=0);

#endif
#ifndef PIXEL_H
#define PIXEL_H

#include <math.h>

// TODO: use U16 and add alpha channel
typedef struct
{
    U8 r, g, b;
} Pixel;

Pixel* grayscale(Arena *arena, Pixel *original);
Pixel* G(Arena *arena, Pixel *pixels, F32 sigma, I32 width, I32 height);

#endif

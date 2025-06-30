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

/* PIXEL MACROS */
#define IS_VALID_PIXEL(p, maxval)\
    ( ( (p.r) > (maxval) || (p.g) > (maxval) || (p.b) > (maxval) ) ? 0 : 1 )

#define PRINT_PIXEL(p)\
    (printf("(%hhu, %hhu, %hhu)\n", (p.r), (p.g), (p.b)))

#define PRINT_PIXEL_MAT(p, w, h)                                    \
    {                                                               \
        puts("[");                                                  \
        for (I32 __y = 0; __y < h; __y++)                           \
        {                                                           \
            printf("  ");                                           \
            for (I32 __x = 0; __x < w; __x++)                       \
            {                                                       \
                printf("(%hhu, %hhu, %hhu), ",                      \
                        (p[__y * w + __x].r),                       \
                        (p[__y * w + __x].g),                       \
                        (p[__y * w + __x].b));                      \
            }                                                       \
            putchar('\n');                                          \
        }                                                           \
        puts("]");                                                  \
    }                                                               \
/* END PIXEL MACROS */ 

#endif

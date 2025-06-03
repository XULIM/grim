#ifndef PPMPROC_H
#define PPMPROC_H

typedef enum
{
    READALL = 1,
} PPM_Proc_Flag;

typedef struct
{
    U8 r, g, b;
} Pixel;

typedef struct
{
    C8 header[2];
    U64 width, height;
    U64 max_color;
    Pixel *pixels;
} PPMImage;

PPMImage* ppm_read(const C8 *restrict path, Arena *restrict arena);

#endif

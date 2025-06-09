#ifndef PPMPROC_H
#define PPMPROC_H

typedef enum
{
    READALL = 1,
} PPM_Proc_Flag;

typedef struct // 3 byte wide
{
    U8 r, g, b;
} Pixel;

typedef struct // 40 byte wide on 64-bit
{
    U64 width, height;
    U64 max_color;
    Pixel *pixels;
    C8 header[2];
} PPMImage;

/* Reads P3 PPM into PPMImage struct, which is allocated on the arena.
 */
PPMImage* ppm_read(const C8 *restrict path, Arena *restrict arena);


U0 ppm_write(const C8 *restrict path, PPMImage *image);

PPMImage* ppm_grayscale(PPMImage *image, Arena *arena);

#endif

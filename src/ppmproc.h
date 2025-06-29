#ifndef PPMPROC_H
#define PPMPROC_H

// TODO: use U16 and add alpha channel
typedef struct
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
 * Returns a pointer to the PPMImage struct on success, NULL otherwise.
 */
PPMImage* ppm_read(const C8 *restrict path, Arena *restrict arena);


/* Writes PPMImage struct to path. */
I32 ppm_write(const C8 *restrict path, PPMImage *image);


/* SIMD accelerated grayscaling using SSE4.2.
 *
 * Grayscales the PPMImage struct, allocates a new PPMImage struct on arena and
 * returns a pointer to it on success. */
PPMImage* ppm_grayscale(PPMImage *image, Arena *arena);

#define IS_VALID_PIXEL(p, maxval)\
    ( ( (p.r) > (maxval) || (p.g) > (maxval) || (p.b) > (maxval) ) ? 0 : 1 )

#endif

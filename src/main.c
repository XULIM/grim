#define NOB_IMPLEMENTATION
#include "nob.h"
#include "allat.h"
#include "arena.h"
#include "pixel.h"
#include "ppmgen.h"
#include "ppmproc.h"

#include <math.h>
#include <immintrin.h>


#define THREE 3

Pixel *create_gray(Arena *arena, I32 gray_values[], U64 n)
{
    Pixel *grays;
    U64 i;
    F32 curr;

    grays = new(arena, Pixel, n, NOZERO);
    for (i = 0; i < n; i++)
    {
        curr = gray_values[i];
        grays[i].r = curr;
        grays[i].g = curr;
        grays[i].b = curr;
    }

    return grays;
}


I32 main(U0)
{
    Arena *arena;

    /*
    PPMImage *ppmimage, *grayimage;
    Pixel *pix;
    I32 status;
    */

    // reserve 10 MB of space
    arena = arena_alloc(10 * 1024 * 1024);

    I32 grid[THREE * THREE] = {100, 150, 100,
                               120, 200, 120,
                               100, 150, 100};
    Pixel *grays = create_gray(arena, grid, THREE * THREE);
    Pixel *blurred = G(arena, grays, 1.0f, THREE, THREE);
    PRINT_PIXEL_MAT(blurred, THREE, THREE);

    // G_3tap(arena);
    /*
    ppmimage = ppm_read(IMG_FOLDER"new.ppm", arena);
    if (ppmimage == NULL)
    {
        nob_log(NOB_ERROR, "could not read "IMG_FOLDER"new.ppm.");
        return -1;
    }

    grayimage = ppm_grayscale(ppmimage, arena);
    if (grayimage == NULL)
    {
        nob_log(NOB_ERROR, "could not convert image to grayscale.");
        return -1;
    }

    status = ppm_write(IMG_FOLDER"gray.ppm", grayimage);
    if (status == -1)
    {
        nob_log(NOB_ERROR, "failed to write "IMG_FOLDER"gray.ppm.");
        return -1;
    }
    */


    arena_release(arena);
    return 0;
}

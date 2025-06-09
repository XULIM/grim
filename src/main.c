#define NOB_IMPLEMENTATION
#include "nob.h"
#include "allat.h"
#include "arena.h"
#include "ppmgen.h"
#include "ppmproc.h"
#include <immintrin.h>


I32 main(U0)
{
    PPMImage *ppmimage, *grayimage;
    Arena *arena;

    // reserve 10 MB of space
    arena = arena_alloc(10 * 1024 * 1024);

    ppmimage = ppm_read(IMG_FOLDER"new.ppm", arena);
    grayimage = ppm_grayscale(ppmimage, arena);

    printf("%2s\n%lu %lu\n%lu\n",
            grayimage->header,
            grayimage->width,
            grayimage->height,
            grayimage->max_color);

    for (U64 i = 0; i < grayimage->width * grayimage->height; i++)
    {
        printf("%hhu %hhu %hhu\n",
                grayimage->pixels[i].r,
                grayimage->pixels[i].g,
                grayimage->pixels[i].b);
    }


    /*
    printf("width: %lu\nheight: %lu\nmax_color: %lu\n",
            ppmimage->width,
            ppmimage->height,
            ppmimage->max_color);

    for (U64 i = 0; i < ppmimage->width * ppmimage->height; i++)
    {
        printf("%hhu %hhu %hhu\n",
                ppmimage->pixels[i].r,
                ppmimage->pixels[i].g,
                ppmimage->pixels[i].b);
    }
    */

    ppm_grayscale(ppmimage, arena);

    arena_release(arena);
    return 0;
}

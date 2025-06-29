#define NOB_IMPLEMENTATION
#include "nob.h"
#include "allat.h"
#include "arena.h"
#include "ppmgen.h"
#include "ppmproc.h"

#include <math.h>
#include <immintrin.h>


#define THREE 3

Pixel *create_gray(Arena *arena, F32 gray_values[], I32 n)
{
    Pixel *grays;
    I32 i;
    F32 curr;

    grays = new(arena, Pixel, n, NOZERO);
    for (i = 0; i < n; i++)
    {
        curr = gray_values[i];
        grays[i] = {curr, curr, curr};
    }

    return grays;
}


U0 G_3tap(Arena *arena)
{
    U64 original_offset = arena->offset;
    const I32 size = 3;
    I32 x, y, k, r = 1;
    I32 grid[THREE * THREE] = {100, 150, 100,
                               120, 200, 120,
                               100, 150, 100};
    F32 kernel[size], sum = 0.0f, s = 1.0f;

    // precompute kernel
    for (x = -r; x <= r; x++)
    {
        kernel[x + r] = exp(-((x*x) / (2 * (s*s)))) /
            sqrtf((2 * M_PI) * (s * s));
        sum += kernel[x + r];
    }

    // normalize kernel values
    printf("kernel:\n");
    for (x = 0; x < size; x++)
    {
        kernel[x] /= sum;
        printf("%f ", kernel[x]);
    }
    printf("\n\n");

    //U8 *temp = new(arena, U8, size, NOZERO);

    // horizontal pass (blur each row)
    F32 aqum, weight;
    I32 px = 0, curr = 0;
    for (y = 0; y < size; y++)
    {
        // iterate thru each row
        for (x = 0; x < size; x++)
        {
            aqum = 0.0;
            // iterate thru kernel
            for (k = -r; k <= r; k++)
            {
                px = x + k;
                px = px >= size ? size - 1 : (px < 0 ? 0 : px);
                curr = grid[y * size + px];

                weight = kernel[k + r];
                aqum += (F32)curr * weight;
            }
            printf("%f ", aqum);
        }
        printf("\n");
    }

    // print
    for (x = 0; x < size; x++)
    {
        for (y = 0; y < size; y++)
        {
            printf("%d ", grid[x + y * size]);
        }
        printf("\n");
    }

    arena->offset = original_offset;
}


U0 G(Arena *arena, F32 s)
{
    U64 original_offset;
    F32 *kernel, sum;
    I32 x, r, size;

    r = ceil(3 * s);
    size = 2 * r + 1;
    sum = 0.0f;
    
    original_offset = arena->offset;

    kernel = new(arena, F32, size, NOZERO);

    // precompute 1D kernel
    for (x = -r; x <= r; x++)
    {
        kernel[x + r] = exp(-((x*x) / (2 * (s*s)))) /
            sqrtf((2 * M_PI) * (s * s));
        sum += kernel[x + r];
    }

    // normalize kernel
    for (x = 0; x < size; x++)
    {
        kernel[x] /= sum;
        printf("%f ", kernel[x]);
    }
    printf("\n");

    arena->offset = original_offset;
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

    G_3tap(arena);

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

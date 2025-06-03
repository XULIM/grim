#include "nob.h"
#include "allat.h"
#include "arena.h"
#include "ppmproc.h"

#include <string.h>

#define NOB_IMPLEMENTATION

#define IS_VALID_PIXEL(p, h)\
    ( (p) > (h) ? 0 : 1 )

PPMImage* ppm_read(const C8 *restrict path, Arena *restrict arena)
{
    U8 r, g, b;
    U16 width, height, maxval;
    U64 original_offset, i;
    FILE *file;
    PPMImage *ppmimage;

    // Open PPM file
    file = fopen(path, "r");
    if (!file)
    {
        nob_log(NOB_ERROR, "could not open file %s.", path);
        fclose(file);
        return NULL;
    }

    // Store current offset to restore to in case of error
    original_offset = arena->offset;
    
    // Create PPMImage struct to store info
    ppmimage = new(arena, PPMImage, 1, NOZERO);

    // Read PPM header (reading for P3 fomrat)
    if (fscanf(file, "%2s", ppmimage->header) != 1 ||
            strcmp(ppmimage->header, "P3") != 0)
    {
        nob_log(NOB_ERROR,
                "invalid format for %s, please provide P3 PPM format.",
                path);
        fclose(file);
        return NULL;
    }
    else
    {
        // Read width, height, and max color value of each pixel
        nob_log(NOB_INFO, "found valid PPM format.");

        fscanf(file, "%"SCNU16" %"SCNU16" %"SCNU16,
                &width, &height, &maxval);
    }

    if (maxval > 255)
    {
        nob_log(NOB_ERROR, "max color value %"PRIU16" exceeds 255.", maxval);
        fclose(file);
        return NULL;
    }

    ppmimage->width = width;
    ppmimage->height = height;
    ppmimage->max_color = maxval;
    ppmimage->pixels = new(arena, Pixel, width * height, NOZERO);

    // Read and store Pixel information
    for (i = 0; i < width * height; i++)
    {
        if (fscanf(file, "%"SCNU8" %"SCNU8" %"SCNU8, &r, &g, &b) != 3)
        {
            // Failed to read Pixel color triplets
            nob_log(NOB_ERROR, "error after %"PRIU64" triplets.", i);
            arena->offset = original_offset;
            fclose(file);
            return NULL;
        }

        // Invalid pixel triplet (out of range)
        if (!IS_VALID_PIXEL(r, maxval) ||
                !IS_VALID_PIXEL(g, maxval) ||
                !IS_VALID_PIXEL(b, maxval))
        {
            nob_log(NOB_ERROR,
                    "invalid pixel at %"PRIU64" (pixel color out of range).",
                    i);
            arena->offset = original_offset;
            fclose(file);
            return NULL;
        }

        ppmimage->pixels[i].r = r;
        ppmimage->pixels[i].g = g;
        ppmimage->pixels[i].b = b;
    }

    fclose(file);
    return ppmimage;
}

#include "nob.h"
#include "allat.h"
#include "arena.h"
#include "ppmproc.h"

#include <string.h>
#include <immintrin.h>

#define NOB_IMPLEMENTATION

#define IS_VALID_PIXEL(p, maxval)\
    ( ( (p.r) > (maxval) || (p.g) > (maxval) || (p.b) > (maxval) ) ? 0 : 1 )

// Allow the functions to manage memory on arena,
// or keep it modular?

PPMImage* ppm_read(const C8 *restrict path, Arena *restrict arena)
{
    U64 original_offset, i;
    FILE *file;
    PPMImage *ppmimage;
    U16 width, height, maxval;

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
        if (fscanf(file, "%"SCNU8" %"SCNU8" %"SCNU8,
                    &ppmimage->pixels[i].r,
                    &ppmimage->pixels[i].g,
                    &ppmimage->pixels[i].b) != 3)
        {
            // Failed to read Pixel color triplets
            nob_log(NOB_ERROR, "error after %"PRIU64" triplets.", i);
            arena->offset = original_offset;
            fclose(file);
            return NULL;
        }

        // Invalid pixel triplet (out of range)
        if (!IS_VALID_PIXEL(ppmimage->pixels[i], maxval))
        {
            nob_log(NOB_ERROR,
                    "invalid pixel at %"PRIU64" (pixel color out of range).",
                    i);
            arena->offset = original_offset;
            fclose(file);
            return NULL;
        }
    }

    fclose(file);
    return ppmimage;
}

PPMImage* ppm_grayscale(PPMImage *image, Arena *arena)
{
    __m128 COEFF_R, COEFF_G, COEFF_B, r, g, b;
    __m128 grays;
    F32 temp[4];
    U64 size, i, j;
    Pixel *gray_pixels;
    PPMImage *gray_image;
    U8 temp_color;

    // Denoting Y as the gray value,
    // it is computed as Y = 0.2989R + 0.5870G + 0.1140B

    COEFF_R = _mm_set1_ps(0.299f);
    COEFF_G = _mm_set1_ps(0.587f);
    COEFF_B = _mm_set1_ps(0.114f);

    size = (image->width) * (image->height);

    gray_pixels = new(arena, Pixel, size, NOZERO);
    gray_image = new(arena, PPMImage, size, NOZERO);

    // Load 4 RGB values at the same time
    for (i = 0; i < size; i += 4)
    {
        r = _mm_set_ps((F32)image->pixels[i].r,
                (F32)image->pixels[i+1].r,
                (F32)image->pixels[i+2].r,
                (F32)image->pixels[i+3].r);
        g = _mm_set_ps((F32)image->pixels[i].g,
                (F32)image->pixels[i+1].g,
                (F32)image->pixels[i+2].g,
                (F32)image->pixels[i+3].g);
        b = _mm_set_ps((F32)image->pixels[i].b,
                (F32)image->pixels[i+1].b,
                (F32)image->pixels[i+2].b,
                (F32)image->pixels[i+3].b);

        // Here: grays = (0.299r + 0.587g) + 0.114b
        grays = _mm_add_ps(_mm_add_ps(
                    _mm_mul_ps(COEFF_R, r),
                    _mm_mul_ps(COEFF_G, g)),
                    _mm_mul_ps(COEFF_B, b));

        _mm_store_ps(temp, grays);

        // Copy F32 gray values from temp to Pixel array
        for (j = 0; j < 4; j++)
        {
            temp_color = (U8)grays[j];
            gray_pixels[i+j].r = temp_color;
            gray_pixels[i+j].g = temp_color;
            gray_pixels[i+j].b = temp_color;
        }
    }

    // Still use P3 since these are RGB values
    strcpy(gray_image->header, "P3");
    gray_image->width = image->width;
    gray_image->height = image->height;
    gray_image->max_color = image->max_color;
    gray_image->pixels = gray_pixels;

    return gray_image;
}

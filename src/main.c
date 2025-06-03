#define NOB_IMPLEMENTATION
#include "nob.h"
#include "allat.h"
#include "arena.h"
#include "ppmgen.h"
#include "ppmproc.h"
#include <immintrin.h>


U0 test(U0)
{
    Arena *arena = arena_alloc(8 * 1024);
    
    float *arr1 = (float*)arena_push(arena, alignof(float),
            3 * sizeof(float), NOZERO);
    float *arr2 = (float*)arena_push(arena, alignof(float),
            3 * sizeof(float), NOZERO);

    arr1[0] = 4.0;
    arr1[1] = 3.0;
    arr1[2] = 2.0;

    arr2[0] = 7.0;
    arr2[1] = 8.0;
    arr2[2] = 9.0;

    __m128 vec1, vec2, sum, mul;
    
    vec1 = _mm_load_ps(arr1);
    vec2 = _mm_load_ps(arr2);

    sum = _mm_add_ps(vec1, vec2);
    mul = _mm_mul_ps(vec1, vec2);

    _mm_storeu_ps(arr1, sum);
    _mm_storeu_ps(arr2, mul);

    printf("Sum\n");
    for (int i = 0; i < 3; i++)
        printf("%.2f ", arr1[i]);
    printf("\n");

    printf("Product\n");
    for (int i = 0; i < 3; i++)
        printf("%.2f ", arr2[i]);
    printf("\n");

    arena_release(arena);
}


I32 main(U0)
{
    PPMImage *ppmimage;
    Arena *arena;

    // reserve 10 MB of space
    arena = arena_alloc(10 * 1024 * 1024);

    ppmimage = ppm_read(IMG_FOLDER"new.ppm", arena);

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

    arena_release(arena);
    return 0;
}

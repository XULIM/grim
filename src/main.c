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
    U8 *triplets;
    U64 i, offset, *size;
    Arena *arena;

    // generate new ppm
    if (ppm_gen("new.ppm") != 0)
    {
        return -1;
    }

    // reserve 10 MB of space
    arena = arena_alloc(10 * 1024 * 1024);

    size = new(arena, U64, 1, NOZERO);

    offset = ppm_read(IMG_FOLDER"test.ppm", size, arena);
    if (offset == 0)
    {
        fprintf(stderr, "AHHHHHHHHHHH\n");
        return 0;
    }

    printf("size: %"PRIU64"\n", *size);

    triplets = (U8*)(arena->buf)+offset;
    for (i = 0; i < *size; i += 3)
    {
        printf("%"PRIU8" %"PRIU8" %"PRIU8"\n",
                triplets[i], triplets[i+1], triplets[i+2]);
    }

    arena_release(arena);
    return 0;
}

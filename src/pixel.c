#include "allat.h"
#include "arena.h"
#include "pixel.h"

Pixel* G(Arena *arena, Pixel *pixels, F32 sigma, I32 width, I32 height)
{
    Pixel curr, *temp, *output;
    U64 original_offset;
    I32 x, px, y, py, k;
    I32 r, size;
    F32 sum, weight, fr, fg, fb;

    r = ceil(3.0f * sigma);
    size = 2 * r + 1;
    sum = 0.0;
    F32 kernel[size];

    // precompute kernel
    for (x = -r; x <= r; x++)
    {
        kernel[x + r] = exp(-((x*x) / (2 * (sigma * sigma)))) /
            sqrtf((2 * M_PI) * (sigma * sigma));
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


    // reserve for output
    output = new(arena, Pixel, width * height, NOZERO);

    original_offset = arena->offset;

    temp = new(arena, Pixel, width * height, NOZERO);

    // horizontal pass
    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            fr = 0.0f, fg = 0.0f, fb = 0.0f;
            for (k = -r; k <= r; k++)
            {
                px = x + k;
                px = px >= width ? width - 1 : (px < 0 ? 0 : px);

                curr = pixels[y * height + px];
                weight = kernel[k + r];

                fr += curr.r * weight;
                fg += curr.g * weight;
                fb += curr.b * weight;
            }
            temp[y * width + x].r = fr;
            temp[y * width + x].g = fg;
            temp[y * width + x].b = fb;
        }
    }
    
    // vertical pass
    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            fr = 0.0f, fg = 0.0f, fb = 0.0f;
            for (k = -r; k <= r; k++)
            {
                py = y + k;
                py = py >= height ? height - 1 : (py < 0 ? 0 : py);

                curr = pixels[py * width + x];
                weight = kernel[k + r];

                fr += curr.r * weight;
                fg += curr.g * weight;
                fb += curr.b * weight;
            }
            output[y * width + x].r = fr;
            output[y * width + x].g = fg;
            output[y * width + x].b = fb;
        }
    }

    arena->offset = original_offset;
    return output;
}

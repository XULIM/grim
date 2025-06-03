#ifndef PPMGEN_H
#define PPMGEN_H

#include <linux/limits.h>

#define IMG_FOLDER "img/"
#define I_COEFF 255.999


/* Generates new P3 PPM image.
 * Returns 0 on success, -1 otherwise.
 * */
I32 PPM_GEN(const char *img_name)
{
    // Based off of https://raytracing.github.io/books/RayTracingInOneWeekend.html
    // See section on PPM file format.

    U16 img_h, img_w;
    U16 i, j, ir, ig, ib;
    F64 r, g, b;
    FILE *ppmfd;
    char img_path[PATH_MAX];

    strcpy(img_path, IMG_FOLDER);
    strcat(img_path, img_name);

    img_w = 256;
    img_h = 256;

    ppmfd = fopen(img_path, "w");
    if (ppmfd == NULL)
    {
        fprintf(stderr, "Could not open file: %s.\n", img_path);
        return -1;
    }

    // write header, width, and height
    // P3
    // width height
    fprintf(ppmfd, "P3\n%"PRIU16" %"PRIU16"\n255\n", img_w, img_h);

    // write color triplets
    for (i = 0; i < img_h; i++)
    {
        for (j = 0; j < img_w; j++)
        {
            r = ((F64)j) / (img_w - 1);
            g = ((F64)i) / (img_h - 1);
            b = 0.0;

            ir = (U16)(I_COEFF * r);
            ig = (U16)(I_COEFF * g);
            ib = (U16)(I_COEFF * b);

            fprintf(ppmfd, "%"PRIU16" %"PRIU16" %"PRIU16"\n",
                    ir, ig, ib);
        }
    }

    return 0;
}

#define ppm_gen(img_name) PPM_GEN(img_name)

#endif

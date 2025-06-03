#ifndef PPMPROC_H
#define PPMPROC_H

/* libpng and zlib for PPM <--> PNG */

/* size = image width * height.
 **/
U64 PPM_READ(const C8 *ppmpath, U64 *size, Arena *restrict arena)
{
    U64 i;
    U64 width, height, maxval, original_offset;
    U8 *triplets;
    C8 *ppmformat;

    // Open ppmfile
    FILE *ppmfile = fopen(ppmpath, "rb");
    if (!ppmfile) return 0;

    // reserve space to read PPM format
    original_offset = arena->offset;
    ppmformat = new(arena, C8, 2, NOZERO);

    // read width and height of P3 or P6 format
    fscanf(ppmfile, "%s\n", ppmformat);
    if (ppmformat[0] == 'P' &&
            (ppmformat[1] == '3' || ppmformat[1] == '6'))
    {
        printf("[INFO]: Found P3 file format.\n");

        fscanf(ppmfile, "%"SCNU64" %"SCNU64" %"SCNU64"\n",
                &width, &height, &maxval);
    }
    else
    {
        fprintf(stderr, "[ERROR]: not a valid PPM format.\n");
        return 0;
    }

    // reset offset
    arena->offset = original_offset;

    // reserve space for triplets and read triplets
    *size = 3 * width * height;
    triplets = new(arena, U8, *size , NOZERO);
    for (i = 0; i < *size; i += 3)
    {
        fscanf(ppmfile, "%"SCNU8" %"SCNU8" %"SCNU8"\n",
                &triplets[i], &triplets[i+1], &triplets[i+2]);
    }
    
    fclose(ppmfile);
    return original_offset;
}

#define ppm_read(ppmpath, size, arena)\
    (PPM_READ((ppmpath), (size), (arena)))

#endif

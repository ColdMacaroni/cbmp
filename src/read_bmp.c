#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "bmp_structs.h"

rgb_t
*read_bmp(const char *restrict filename, int32_t *width, int32_t *height)
{
    rgb_t *arr = NULL;
    header_t header;

    FILE *file = fopen(filename, "rb");

    // Get the bmp header, itll always be 14 bytes.
    fread(&header, 1, 14, file);

    // Check that it is in fact a bitmap. Or at least it says it is.
    if (header.header_field[0] != 'B' || header.header_field[1] != 'M')
    {
        fprintf(stderr, "File format not supported\n");
        return NULL;
    }


    return arr;
};

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "bmp_structs.h"

#define cleanup(file) do {fclose(file);return NULL;} while(0)
rgb_t
*read_bmp(const char *restrict filename, int32_t *width, int32_t *height)
{
    /* Only bmp supported is BM header, BITMAPINFOHEADER dib, 24bit rgb data */
    enum {header_nmb = 14, dib_nmb = 40};

    FILE *file = fopen(filename, "rb");
    if (file == NULL)
        return NULL;

    rgb_t *arr = NULL;

    // Get the bmp header
    header_t header;
    fread(&header, 1, header_nmb, file);

    // Check that it is in fact a bitmap. Or at least that says it is.
    if (header.header_field[0] != 'B' || header.header_field[1] != 'M')
    {
        fprintf(stderr, "File format not supported. Header field specifies %c%c\n",
                header.header_field[0], header.header_field[1]);

        cleanup(file);
    }

    // Get the dib, only BITMAPINFOHEADER as of now
    dib_t dib;
    fread(&dib, 1, dib_nmb, file);

    // Do a bunch of checks to make sure this is real!
    // Quick check that'll rule a bunch of stuff out'
    if (dib.dib_size != dib_nmb)
    {
        fprintf(stderr, "Unknown DIB header. Not supported. Size = %d\n", dib.dib_size);

        cleanup(file);
    }
    if (dib.color_planes != 1)
    {
        fprintf(stderr, "Color planes is not 1, it's %d. No idea what to do.\n", dib.color_planes);

        cleanup(file);
    }


    *width = dib.width;
    *height = dib.height;

    return arr;
}
#undef cleanup

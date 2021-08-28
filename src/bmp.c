#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* To spread an int into over multiple bytes
 * TODO: Turn this into a macro maybe?
 * Int. 4 bytes. Will reset leftover bits.
 *    *(int *)&content[0] = (int)0x4e494345;
 *    4e494345 -> 4e 49 43 45 -> 'N' 'I' 'C' 'E'
 *    content[0] == 'E'
 *    content[1] == 'C'
 *    content[2] == 'I'
 *    content[3] == 'N'
 *
 * Short int. 2 bytes. Will reset leftover bits.
 *    *(short int *)&content[0] = (short int)0x4849;
 *    4849 -> 48 49 -> 'H' 'I'
 *    content[0] == 'I'
 *    content[1] == 'H'
 */


int flip_int(int int_in)
{
    /* 0x11223344 -> 0x22114433 */
    // I tried to do some bitshifting with masks but it ended up setting the
    // first byte to 0xff if the second was 0x00. This is easier /shrug

    char in[4];
    char out[4];

    int int_out;

    *(int *)&in[0] = int_in;

    out[0] = in[1];
    out[1] = in[0];
    out[2] = in[3];
    out[3] = in[2];

    int_out = *(int *)&out[0];

    /*
    printf("in:\t0x08%x", int_in);
    printf("out:\t0x08%x", int_out);
    //*/
    return int_out;
}

char
*create_bmp_header(size_t *nmb, unsigned int nmb_dib, unsigned int nmb_data)
{
    // Bitmap header has a fixed size of 14 bytes
    enum { nmb_header = 14 };
    char *header = calloc(sizeof(char), nmb_header);

    if (header == NULL)
        return header;

    /* https://en.wikipedia.org/wiki/BMP_file_format#Bitmap_file_header */
    // ID. 2 bytes. 0-1
    header[0] = 'B';
    header[1] = 'M';

    // Size. 4 bytes. 2-5
    unsigned int bytes = nmb_header + nmb_dib + nmb_data;
    *(int *)&header[2] = flip_int((unsigned int)bytes);

    // Reserved. 4 bytes. 6-9
    *(int *)&header[4] = (int)0;

    // Pixel Array Offset. 4 bytes. 10-14
    unsigned int offset = nmb_header + nmb_dib;
    *(int *)&header[10] = flip_int((unsigned int)offset);

    if (nmb != NULL)
        *nmb += nmb_header;

    return header;
}

char
*create_min_dib(size_t *nmb, signed int height, signed int width)
{
    /* https://en.wikipedia.org/wiki/BMP_file_format#DIB_header_(bitmap_information_header) */
    // The smallest dib is 40 bytes. BITMAPINFOHEADER
    enum { nmb_dib = 40 };
    char *dib = calloc(sizeof(char), nmb_dib);

    if (dib == NULL)
        return NULL;

    // Size of header. 4 bytes. 0-3
    *(int *)&dib[0] = flip_int((int)nmb_dib);

    // Width, 4 bytes. 4-7
    // Height, 4 bytes, 8-11

    if (nmb != NULL)
        *nmb = nmb_dib;

    return dib;
}

char
*create_bmp_1bit(size_t *nmb_arg, const bool *arr, signed int width, signed int height)
{
    size_t header_nmb = 0;
    size_t dib_nmb = 0;
    size_t data_nmb = 0;

    size_t fallback_nmb = 0;
    size_t *nmb;

    // Accept NULL for argument.
    if (nmb_arg == NULL)
        nmb = &fallback_nmb;
    else
        nmb = nmb_arg;

    // Create data

    // Create DIB

    // Create header last because it needs dib and data nmb
    char *header = create_bmp_header(&header_nmb, dib_nmb, data_nmb);

    *nmb += header_nmb + dib_nmb + data_nmb;
    char *bmp = calloc(sizeof(char), *nmb);

    // Cat header
    bmp = strncat(bmp, header, header_nmb);
    free(header);

    return bmp;
}

// Assuming char is 4 bits,
// int to shift = i % 4
// content[sometginf] = content[sometginf] & ~(1 << to_shift) | arr[i]<<to_shift
// Should set the bit, the first & will reset it

size_t
write_bmp_1bit(char *restrict filename, const bool *arr, const int width, const int height)
{
    // Code
    FILE *file = fopen(filename, "wb+");
    size_t nmb = 0;

    char *bmp_content = create_bmp_1bit(&nmb, arr, width, height);

    size_t fwrite_result = fwrite(bmp_content, sizeof(char), nmb, file);

    free(bmp_content);
    fclose(file);
    return fwrite_result;
}

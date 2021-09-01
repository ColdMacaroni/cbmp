#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
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


uint8_t bool_arr_to_int8(const bool *restrict arr)
{
    uint8_t out = 0;

    // arr left to right are bits left to right.
    for (int i = 0; i < 8; i++)
    {
        out <<= 1;
        out |= arr[i];
    }

    return out;
}


int32_t flip_int(int32_t int_in)
{
    /* 0x11223344 -> 0x22114433 */
    // I tried to do some bitshifting with masks but it ended up setting the
    // first byte to 0xff if the second was 0x00. This is easier /shrug

    int8_t in[4];
    int8_t out[4];

    int32_t int_out;

    *(int32_t *)&in[0] = int_in;

    out[0] = in[1];
    out[1] = in[0];
    out[2] = in[3];
    out[3] = in[2];

    int_out = *(int32_t *)&out[0];

    /*
    printf("in:\t0x08%x", int_in);
    printf("out:\t0x08%x", int_out);
    //*/
    return int_out;
}

int16_t flip_short_int(int16_t int_in)
{
    /* 0x11223344 -> 0x22114433 */
    // I tried to do some bitshifting with masks but it ended up setting the
    // first byte to 0xff if the second was 0x00. This is easier /shrug

    int8_t in[2];
    int8_t out[2];

    int16_t int_out;

    *(short int *)&in[0] = int_in;

    out[0] = in[1];
    out[1] = in[0];

    int_out = *(int16_t *)&out[0];

    /*
    printf("in:\t0x08%x", int_in);
    printf("out:\t0x08%x", int_out);
    //*/
    return int_out;
}

int8_t
*create_bmp_header(size_t *nmb, unsigned int nmb_dib, unsigned int nmb_data)
{
    // Bitmap header has a fixed size of 14 bytes
    enum { nmb_header = 14 };
    int8_t *header = calloc(sizeof(int8_t), nmb_header);

    if (header == NULL)
        return header;

    /* https://en.wikipedia.org/wiki/BMP_file_format#Bitmap_file_header */
    // ID. 2 bytes. 0-1
    header[0] = 'B';
    header[1] = 'M';

    // Size. 4 bytes. 2-5
    uint32_t size = nmb_header + nmb_dib + nmb_data;
    *(uint32_t *)&header[2] = /*flip_int*/((uint32_t)size);

    // Reserved. 4 bytes. 6-9
    *(uint32_t *)&header[4] = (uint32_t)0;

    // Pixel Array Offset. 4 bytes. 10-14
    unsigned int offset = nmb_header + nmb_dib;
    *(uint32_t *)&header[10] = /*flip_int*/((uint32_t)offset);

    if (nmb != NULL)
        *nmb += nmb_header;

    return header;
}

int8_t
*create_dib_1bit(size_t *nmb, size_t data_nmb, signed int height, signed int width)
{
    /* Creates smallest possible dib */
    /* https://en.wikipedia.org/wiki/BMP_file_format#DIB_header_(bitmap_information_header) */
    // The smallest dib is 40 bytes. BITMAPINFOHEADER
    enum { nmb_dib = 40 };
    int8_t *dib = calloc(sizeof(int8_t), nmb_dib);

    if (dib == NULL)
        return NULL;

    // Size of header. 4 bytes. 0-3
    *(int32_t *)&dib[0] = /*flip_int*/((int32_t)nmb_dib);

    // Width, 4 bytes. 4-7
    *(int32_t *)&dib[4] = /*flip_int*/((int32_t)width);

    // Height, 4 bytes, 8-11
    *(int32_t *)&dib[8] = /*flip_int*/((int32_t)height);

    // Color plane. 2 bytes. 12-13
    *(int32_t *)&dib[12] = /*flip_short_int*/((int16_t)1);

    // Bits per pixel. 2 bytes. 14-15
    *(int32_t *)&dib[14] = /*flip_short_int*/((int16_t)1);

    // Compression. 4 bytes. 16-19
    *(int32_t *)&dib[16] = /*flip_int*/((int32_t)0);

    // Size of data. 4 bytes. 20-23
    *(int32_t *)&dib[20] = /*flip_int*/((int32_t)data_nmb);

    // Resolution
    const uint32_t resol = 2835;

    // Vertical Resol. 4 bytes. 24-27
    *(uint32_t *)&dib[24] = /*flip_int*/((uint32_t)resol);

    // Horizontal Resol. 4 bytes. 28-31
    *(uint32_t *)&dib[28] = /*flip_int*/((uint32_t)resol);

    // Colors in palette. 4 bytes. 32-35
    *(uint32_t *)&dib[32] = /*flip_int*/((uint32_t)0);

    // Important colors. 4 bytes. 36-39
    *(uint32_t *)&dib[36] = /*flip_int*/((uint32_t)0);

    if (nmb != NULL)
        *nmb += nmb_dib;

    return dib;
}

uint8_t
*create_bmp_data_1bit(size_t *nmb, const bool *arr, int width, int height)
{
    // https://en.wikipedia.org/wiki/BMP_file_format#Pixel_storage
    uint8_t *data = calloc(sizeof(uint8_t), (((width + 31) / 32) * 4) * height);
    int byte;
    const int byte_bits = 8;
    const int bytes_per_row = (width+31)/32;
    printf("bpr %d\nh %d\n", bytes_per_row, height);
    // Build from bottom up
    for (int row = height - 1; row >= 0; row--)
    {
        for (byte = 0; byte < bytes_per_row; byte++)
        {
            data[height*bytes_per_row + byte] = bool_arr_to_int8(&(arr[height * row + byte * byte_bits]));
        }

        /*
        if (column >= width - (width % 4))
        {
            // Add padding
        }
        */
    }

    if (nmb != NULL)
        *nmb += bytes_per_row * height;
    //*nmb=2;

    printf("%lu\n", *nmb);

    // definitely nothing can go wrong.
    return data;
}

int8_t
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
    uint8_t *data = create_bmp_data_1bit(&data_nmb, arr, width, height);

    // Create DIB
    int8_t *dib = create_dib_1bit(&dib_nmb, data_nmb, width, height);

    // Create header last because it needs dib and data nmb
    int8_t *header = create_bmp_header(&header_nmb, dib_nmb, data_nmb);

    *nmb += header_nmb + dib_nmb + data_nmb;
    int8_t *bmp = calloc(sizeof(int8_t), *nmb);

    // Cat header. Cant use strncat because \0 have to be preserved
    for (size_t i = 0; i < header_nmb; i++)
        bmp[i] = header[i];

    free(header);

    // Cat dib
    for (size_t i = 0; i < dib_nmb; i++)
        bmp[header_nmb + i] = dib[i];

    free(dib);

    // Cat data
    for (size_t i = 0; i < data_nmb; i++)
        bmp[header_nmb + dib_nmb + i] = data[i];

    free(data);

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

    int8_t *bmp_content = create_bmp_1bit(&nmb, arr, width, height);

    size_t fwrite_result = fwrite(bmp_content, sizeof(int8_t), nmb, file);

    free(bmp_content);
    fclose(file);
    return fwrite_result;
}

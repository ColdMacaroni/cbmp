#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "bmp_structs.h"

rgb_t
*bool_to_rgb_arr(const bool *arr, unsigned int len)
{
    rgb_t *rgb_arr = calloc(sizeof(rgb_t), len);

    const rgb_t white = { .r = 0xff, .g = 0xff, .b = 0xff};
    const rgb_t black = { .r = 0x00, .g = 0x00, .b = 0x00};

    for (unsigned int i = 0; i < len; i++)
        if (arr[i])
            rgb_arr[i] = white;
        else
            rgb_arr[i] = black;

    return rgb_arr;
}
/* Function not used, kept for why not.
uint32_t
bool_arr_to_uint32(const bool *arr, unsigned int arr_len)
{
    uint32_t out = 0;

    // arr left to right are bits left to right.
    for (unsigned int i = 0; i < arr_len; i++)
    {
        out <<= 1;
        out |= arr[i];
    }

    // Pad
    out <<= 32 - arr_len;

    return out;
}
*/

header_t
create_bmp_header(size_t *nmb, size_t nmb_dib, size_t nmb_data)
{
    header_t header;
    enum {nmb_header = 14};

    assert(sizeof(header_t) == nmb_header);

    // Show this as bitmap.
    header.header_field[0] = 'B';
    header.header_field[1] = 'M';

    // Size of file in bytes.
    header.size = nmb_header + nmb_dib + nmb_data;

    // No idea what this is for
    header.reserved = 0;

    // Bytes to pix array
    header.pixarr_offset = nmb_header + nmb_dib;

    *nmb += nmb_header;

    return header;
}

dib_t
create_bmp_dib(size_t *nmb, size_t nmb_data,
               int32_t width, int32_t height,
               uint16_t bit_depth)
{
    dib_t dib;
    enum {nmb_dib = 40, res = 2835};

    assert(sizeof(dib_t) == nmb_dib);
    dib.dib_size = nmb_dib;

    dib.width = width;
    dib.height = height;

    dib.color_planes = 1;
    dib.bit_depth = bit_depth;

    // Not even gonna try to implement this
    dib.compression = 0;

    dib.data_size = nmb_data;

    dib.h_res = res;
    dib.v_res = res;

    // no idea what these are
    dib.color_palette = 0;
    dib.important_colors = 0;

    *nmb += nmb_dib;

    return dib;
}

data_t
create_bmp_data(const rgb_t *arr, int32_t width, int32_t height)
{
    data_t data;
    enum { bits=32, byte_len=3};
    // https://en.wikipedia.org/wiki/BMP_file_format#Pixel_storage
    size_t nmb = (((24 * width + bits - 1) / bits) * 4) * height;

    data.data = malloc(nmb);
    data.nmb = nmb;
    int data_idx = 0;

    int x, y;
    int offset = 0;
    int overflow = 0;
    rgb_t c_rgb;  // Current

    // Maybe uint32_t[2] and have an uint64_t????
    union {
        uint32_t full;
        uint8_t byte[byte_len + 1];
    } dword, next_dword;
    dword.full = 0;
    next_dword.full = 0;

    /* dword.byte[0] = arr[0].b;
     * dword.byte[1] = arr[0].g;
     * dword.byte[2] = arr[0].r;
     *
     * data.data[0] = dword.full;
     */

    // Go the other way because that just how bitmaps are
    // RGB stored as BGR (yikes)
    for (y = height - 1; y >= 0; y--)
    {
        for (x = 0; x < width; x++)
        {
            overflow = 0;

            // Set the current for quick access
            c_rgb = arr[y*width + x];
            switch(offset)
            {
                // Case cascading will take care of writing as many as can be fit
                case 0:
                case 1:
                    dword.byte[offset + 2] = c_rgb.r;
                case 2:
                    dword.byte[offset + 1] = c_rgb.g;
                case 3:
                    dword.byte[offset] = c_rgb.b;
                    break;

                // This should NEVER happen.
                default:
                    assert(0);
            }

            // It'll always be one less than offset unless thats 0
            overflow = (offset) ? offset - 1 : offset;

            // Update with the spaces just occupied
            offset += 3;
            offset %= 4;

            // Write the overflown stuff to the next dword
            switch (overflow)
            {
                // These are the only possible cases, any other means that
                // something else has gone wrong somewhere else.
                case 2:
                    next_dword.byte[overflow - 2] = c_rgb.g;
                case 1:
                    next_dword.byte[overflow - 1] = c_rgb.r;
            }

            // When the current dword is full
            // Offset of 0 means it was *just* filled, overflow clearly means
            // its full.
            if (offset == 0 || overflow)
            {
                // Store the now full dword
                data.data[data_idx++] = dword.full;

                // Update current dword to the next one
                dword.full = next_dword.full;
                offset = overflow;

                next_dword.full = 0;
                overflow = 0;
            }
        }

        // Add any leftovers that did not manage to fill a whole dword
        if (offset)
            data.data[data_idx++] = dword.full;

        // Reset everything
        dword.full = 0;
        next_dword.full = 0;
        offset = 0;
        overflow = 0;
    }

    return data;
}

size_t
write_bmp(char *restrict filename, const rgb_t *arr, int32_t width, int32_t height)
{
    FILE *file = fopen(filename, "wb+");
    size_t nmb_header = 0;
    size_t nmb_dib = 0;
    size_t result = 0;

    data_t data = create_bmp_data(arr, width, height);
    dib_t dib = create_bmp_dib(&nmb_dib, data.nmb, width, height, 24);
    header_t header = create_bmp_header(&nmb_header, nmb_dib, data.nmb);

    result += fwrite(&header, 1, nmb_header, file);
    result += fwrite(&dib, 1, nmb_dib, file);
    result += fwrite(data.data, 1, data.nmb, file);

    free(data.data);
    fclose(file);

    return result;
}

size_t
write_bmp_bool(char *restrict filename, const bool *arr, int32_t width, int32_t height)
{
    rgb_t *rgb_arr = bool_to_rgb_arr(arr, abs(width) * abs(height));
    size_t result = write_bmp(filename, rgb_arr, width, height);

    free(rgb_arr);
    return result;
}

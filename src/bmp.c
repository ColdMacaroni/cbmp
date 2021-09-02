#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

// kdevelop shows warning unless theres a semicolon here... thanks https://blog.csdn.net/qq_38777624/article/details/107022865
;// Pack 1 to make sure these are right next to each other
#pragma pack(push,1)
typedef struct
{
    uint8_t header_field[2];
    uint32_t size;
    uint32_t reserved;
    uint32_t pixarr_offset;
} header_t;

typedef struct
{
    uint32_t dib_size;
    int32_t width;
    int32_t height;
    uint16_t color_planes;
    uint16_t bit_depth;
    uint32_t compression;
    uint32_t data_size;
    int32_t h_res;
    int32_t v_res;
    uint32_t color_palette;
    uint32_t important_colors;
} dib_t;
#pragma pack(pop)

typedef struct
{
    uint32_t *data;
    size_t nmb;
} data_t;

typedef struct
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgb_t;

rgb_t
*bool_to_rgb_arr(const bool *arr, unsigned int len)
{
    rgb_t *rgb_arr = calloc(sizeof(rgb_t), len);

    const rgb_t white = { .r = 0xff, .g = 0xff, .b = 0xff};
    const rgb_t black = { .r = 0x00, .g = 0x00, .b = 0x00};

    for (int i = 0; i < len; i++)
        if (arr[i])
            rgb_arr[i] = white;
        else
            rgb_arr[i] = black;

    return rgb_arr;
}

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
    enum { bits=32 };
    // https://en.wikipedia.org/wiki/BMP_file_format#Pixel_storage
    size_t nmb = (((24 * width + 31) / 32) * 4) * height;

    data.data = malloc(nmb);
    data.nmb = nmb;

    int x, y;
    rgb_t temp_rgb;

    union {
        uint32_t full;
        uint8_t byte[4];
    } dword, next_dword;
    dword.full = 0;
    next_dword.full = 0;

    /* dword.byte[0] = arr[0].b;
     * dword.byte[1] = arr[0].g;
     * dword.byte[2] = arr[0].r;
     *
     * data.data[0] = dword.full;
     */

    int offset = 0;
    int data_idx = 0;

    // Go the other way because that just how bitmaps are
    for (y = height - 1; y >= 0; y--)
    {
        for (x = 0; x < width; x++)
        {
            temp_rgb = arr[y * width + x];

            printf("%02x %02x %02x\n", temp_rgb.r, temp_rgb.g, temp_rgb.b);

            // Set specific bytes depending on how much of the dword is free
            // Theyre set in BGR order.
            switch (offset)
            {
                case 0:
                case 1:
                    dword.byte[offset + 2] = temp_rgb.r;
                case 2:
                    dword.byte[offset + 1] = temp_rgb.g;
                case 3:
                    dword.byte[offset + 0] = temp_rgb.b;
                    break;
                // this should never happen.
                default:
                    assert(0);
            }
            // New offset and wrap around
            offset += 3;
            offset %= 4;

            printf("\n %d \n", offset);

            // When the off is 0, its a new dword.
            if (!offset)
            {
                data.data[data_idx++] = dword.full;
                printf("-- %08x\n", dword.full);
                dword.full = 0;
            }
        }
        // Some might be left in the dword
        if (offset)
        {
            data.data[data_idx++] = dword.full;
            printf("-- %08x\n", dword.full);
            dword.full = 0;
        }
    }

    return data;
}

size_t
write_bmp_bool(char *restrict filename, const bool *arr, int32_t width, int32_t height)
{
    // Code
    FILE *file = fopen(filename, "wb+");
    size_t nmb_header = 0;
    size_t nmb_dib = 0;
    size_t result = 0;
    // int8_t *bmp_content = create_bmp_1bit(&nmb, arr, width, height);

    rgb_t *rgb_arr = bool_to_rgb_arr(arr, width * height);

    data_t data = create_bmp_data(rgb_arr, width, height);
    dib_t dib = create_bmp_dib(&nmb_dib, data.nmb, width, height, 24);
    header_t header = create_bmp_header(&nmb_header, nmb_dib, data.nmb);

    result += fwrite(&header, 1, nmb_header, file);
    result += fwrite(&dib, 1, nmb_dib, file);
    result += fwrite(data.data, 1, data.nmb, file);

    fclose(file);

    return result;
}

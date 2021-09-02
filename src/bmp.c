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

uint32_t
bool_arr_to_uint32(const bool *restrict arr, unsigned int arr_len)
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
create_bmp_data_1bit(const bool* arr, int32_t width, int32_t height)
{
    data_t data;

    // https://en.wikipedia.org/wiki/BMP_file_format#Pixel_storage
    size_t nmb = (((width + 31) / 32) * 4) * height;

    data.data = malloc(nmb);
    data.nmb = nmb;



    return data;
}

size_t
write_bmp_1bit(char *restrict filename, const bool *arr, int32_t width, int32_t height)
{
    // Code
    FILE *file = fopen(filename, "wb+");
    size_t nmb_header = 0;
    size_t nmb_dib = 0;
    size_t nmb_data = 0;
    size_t result = 0;
    // int8_t *bmp_content = create_bmp_1bit(&nmb, arr, width, height);

    dib_t dib = create_bmp_dib(&nmb_dib, nmb_data, width, height, 1);
    header_t header = create_bmp_header(&nmb_header, nmb_dib, nmb_data);

    result += fwrite(&header, 1, nmb_header, file);
    result += fwrite(&dib, 1, nmb_dib, file);

    fclose(file);

    return result;
}

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


size_t
write_bmp_1bit(char *restrict filename, const bool *arr, const int width, const int height)
{
    // Code
    FILE *file = fopen(filename, "wb+");
    size_t nmb = 0;

    // int8_t *bmp_content = create_bmp_1bit(&nmb, arr, width, height);

    header_t header = create_bmp_header(&nmb, 40, 16);

    size_t fwrite_result = fwrite(&header, 1, nmb, file);

    fclose(file);

    return fwrite_result;
}

#ifndef __BMP_STRUCTS__
#define __BMP_STRUCTS__ 1

#include <stddef.h>
#include <stdint.h>

// kdevelop shows warning unless theres a semicolon here... thanks https://blog.csdn.net/qq_38777624/article/details/107022865
// I *could* use pragma push and pop BUT that would need another ; for the push.
// This will make it file wide but its a small price to pay
#pragma clang diagnostic ignored "-Wextra-semi"

// Pack 1 to make sure these are right next to each other
;
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
#endif

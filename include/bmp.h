#ifndef BMP_PROTECT
#define BMP_PROTECT 1
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

; // kdevelop shows warning unless theres a semicolon here... thanks https://blog.csdn.net/qq_38777624/article/details/107022865
// Pack 1 to make sure these are right next to each other
#pragma pack(push,1)
typedef struct {
    uint8_t header_field[2];
    uint32_t size;
    uint32_t reserved;
    uint32_t pixarr_offset;
} header_t;
#pragma pack(pop)

uint8_t
bool_arr_to_int8(bool *arr);

size_t
write_bmp_1bit(char *filename, const bool *arr, const int width, const int height);

#endif

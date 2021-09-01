#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
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


size_t
write_bmp_1bit(char *restrict filename, const bool *arr, const int width, const int height)
{
    // Code
    FILE *file = fopen(filename, "wb+");
    size_t nmb = 0;

    /*
    int8_t *bmp_content = create_bmp_1bit(&nmb, arr, width, height);

    size_t fwrite_result = fwrite(bmp_content, sizeof(int8_t), nmb, file);

    free(bmp_content);
    fclose(file);
    */
    //return fwrite_result;
}

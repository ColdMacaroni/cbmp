#ifndef BMP_PROTECT
#define BMP_PROTECT 1
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

uint8_t
bool_arr_to_int8(bool *arr);

size_t
write_bmp_1bit(char *filename, const bool *arr, const int width, const int height);

#endif

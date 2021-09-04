#ifndef __BMP__
#define __BMP__ 1
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

uint8_t
bool_arr_to_int8(bool *arr);

size_t
write_bmp_bool(char *filename, const bool *arr, const int width, const int height);

#endif

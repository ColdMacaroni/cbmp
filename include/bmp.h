#ifndef __BMP__
#define __BMP__ 1
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "bmp_structs.h"

rgb_t
*bool_to_rgb_arr(const bool *arr, unsigned int len);

size_t
write_bmp(char *restrict filename, const rgb_t *arr, int32_t width, int32_t height);

size_t
write_bmp_bool(char *filename, const bool *arr, const int width, const int height);

#endif

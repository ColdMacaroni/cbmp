#ifndef BMP_PROTECT
#define BMP_PROTECT 1
#include <stddef.h>
#include <stdbool.h>

size_t
write_bmp_1bit(char *restrict filename, const bool *arr, const int width, const int height);

#endif

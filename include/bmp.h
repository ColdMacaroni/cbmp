#ifndef BMP_PROTECT
#define BMP_PROTECT 1
#include <stdbool.h>

size_t
write_bmp(char *restrict filename, const bool *arr, const int width, const int height);

#endif

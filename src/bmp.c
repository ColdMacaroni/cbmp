#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

char
*create_bmp(const bool *arr, const int width, const int height)
{
    char *content = calloc(sizeof(char), 20);

    content = "balls";

    return content;
}

// Assuming char is 4 bits,
// int to shift = i % 4
// content[sometginf] = content[sometginf] & ~(1 << to_shift) | arr[i]<<to_shift
// Should set the bit, the first & will reset it

size_t
write_bmp(char *restrict filename, const bool *arr, const int width, const int height)
{
    // Code
    FILE *file = fopen(filename, "wb+");
    char *bmp_content = create_bmp(arr, width, height);

    size_t fwrite_result = fwrite(bmp_content, sizeof(char), strlen(bmp_content), file);

    free(bmp_content);
    fclose(file);
    return fwrite_result;
}

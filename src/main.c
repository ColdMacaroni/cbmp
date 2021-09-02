/* Convert an array to a 1bit bmp */
#include <stdio.h>
#include <stdbool.h>
#include "bmp.h"

// Test array
#if 1
#define T_WIDTH 10
#define T_HEIGHT 10
const bool g_arr[T_WIDTH * T_HEIGHT] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
};
#endif

void
print_arr(const bool *arr, const int width, const int height);

int
main(/*int argc, char **argv*/)
{
#ifdef T_WIDTH
    print_arr(g_arr, T_WIDTH, T_HEIGHT);

    //printf("\n%lu bytes written\n", write_bmp_1bit("test", g_arr, T_WIDTH, T_HEIGHT));
#endif
    bool wiki_arr[] = {1, 0, 0, 1};
    int width = 2, height = 2;
    printf("\n%lu bytes written\n", write_bmp_bool("test", wiki_arr, width, height));

    return 0;
}

void
print_arr(const bool *arr, const int width, const int height)
{
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
            printf("%d ", arr[(row * width) + col]);
        putchar('\n');
    }
}

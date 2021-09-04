/* Convert an array to a 1bit bmp */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "bmp.h"

// Test array
#define T_WIDTH 10
#define T_HEIGHT 10
const bool g_arr[T_WIDTH * T_HEIGHT] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 1, 1, 0, 1, 1, 0, 1, 1, 0,
    0, 1, 1, 0, 1, 1, 0, 1, 1, 0,
    0, 1, 1, 0, 1, 1, 0, 1, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 1, 0, 1, 1, 1, 1, 0, 1, 0,
    0, 1, 1, 0, 0, 0, 0, 1, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

void
print_arr(const bool *arr, const int width, const int height);

int
main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Please provide filename as argument");
        return EXIT_FAILURE;
    }
    /*
    print_arr(g_arr, T_WIDTH, T_HEIGHT);

    printf("\n%lu bytes written\n", write_bmp_bool(argv[1], g_arr, T_WIDTH, T_HEIGHT));
    */

    int32_t width = 0, height = 0;

    rgb_t *arr = read_bmp(argv[1], &width, &height);

    return EXIT_SUCCESS;
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


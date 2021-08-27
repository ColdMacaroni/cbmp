/* Convert an array to a 1bit bmp */
#include <stdio.h>
#include <stdbool.h>

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

void
print_arr(const bool *arr, const int width, const int height);

int
main(/*int argc, char **argv*/)
{
    print_arr(g_arr, T_WIDTH, T_HEIGHT);
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

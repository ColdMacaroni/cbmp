#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

char
*create_bmp_header(size_t *nmb, unsigned int nmb_dib, unsigned int nmb_data)
{
    // Bitmap header has a fixed size of 14 bytes, one more for null
    enum { nmb_header = 14 };
    char *header = calloc(sizeof(char), nmb_header + 1);
    memset(header, '\0', nmb_header + 1);

    /* https://en.wikipedia.org/wiki/BMP_file_format#Bitmap_file_header */
    // ID. 2 bytes. 0-1
    header[0] = 'B';
    header[1] = 'M';

    // Size. 4 bytes. 2-5
    unsigned int bytes = nmb_dib + nmb_data + nmb_header;
    *(int *)&header[2] = (int)bytes;

    // Reserved. 4 bytes. 6-9

    *nmb += nmb_header;

    return header;
}

/* To spread an int into over multiple bytes
 *
 * Int. 4 bytes. Will reset leftover bits.
 *    *(int *)&content[0] = (int)0x4e494345;
 *    4e494345 -> 4e 49 43 45 -> 'N' 'I' 'C' 'E'
 *    content[0] == 'E'
 *    content[1] == 'C'
 *    content[2] == 'I'
 *    content[3] == 'N'
 *
 * Short int. 2 bytes. Will reset leftover bits.
 *    *(short int *)&content[0] = (short int)0x4849;
 *    4849 -> 48 49 -> 'H' 'I'
 *    content[0] == 'I'
 *    content[1] == 'H'
 */

char
*create_bmp(size_t *nmb, const bool *arr, const int width, const int height)
{
    /*
    char *content = calloc(sizeof(char), 20);
    memset(content, 0, 20);


    *(int *)&content[0] = (int)0x4e494345;

    printf("%c\n", content[0]);
    printf("%c\n", content[1]);
    printf("%c\n", content[2]);
    printf("%c\n", content[3]);
    //*/

    char *header = create_bmp_header(nmb, 40, 16);

    return header;
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
    size_t nmb = 0;

    char *bmp_content = create_bmp(&nmb, arr, width, height);

    size_t fwrite_result = fwrite(bmp_content, sizeof(char), nmb, file);

    free(bmp_content);
    fclose(file);
    return fwrite_result;
}

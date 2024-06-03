#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const int block_size = 512;

int main(int argc, char *argv[])
{
    // Check for only 1 command line argument
    if (argc != 2)
    {
        printf("Usage: ./recover <filename>\n");
        return 1;
    }

    // Open file in read mode
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Unable to open input image file.\n");
        return 1;
    }

    // Create a buffer of 512 bytes
    uint8_t buff[block_size];

    char file_name[8];
    int count = 0;
    FILE *outfile = NULL;

    // Read from input file into buffer
    while (fread(buff, block_size, 1, input))
    {
        // Check first 4 bytes for JPEG header
        // If matched, close existing file, create new file and write to it
        if (buff[0] == 0xff && buff[1] == 0xd8 && buff[2] == 0xff && (buff[3] & 0xf0) == 0xe0)
        {
            if (outfile != NULL)
                fclose(outfile);

            sprintf(file_name, "%03d.jpg", count);
            outfile = fopen(file_name, "w");
            if (outfile == NULL)
            {
                fclose(input);
                printf("Unable to create output file %s.\n", file_name);
                return 1;
            }
            fwrite(buff, block_size, 1, outfile);
            count++;
        }

        // If not matched, write to existing file
        else if (count > 0)
        {
            fwrite(buff, block_size, 1, outfile);
        }
    }

    if (count == 0)
        printf("No images found\n");

    if (outfile != NULL)
        fclose(outfile);

    fclose(input);
    return 0;
}


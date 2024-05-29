#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int h;
    do
    {
        h = get_int("Enter Height between 1 and 8: ");
    }
    while (h < 1 || h > 8);

    for (int i = 0; i < h; i++)
    {
        int j;
        for (j = 0; j < (h - i - 1); j++)
        {
            printf(" ");
        }
        for (j = 0; j < (i + 1); j++)
        {
            printf("#");
        }

        printf("  ");

        for (j = 0; j < (i + 1); j++)
        {
            printf("#");
        }

        printf("\n");
    }

    return 0;
}

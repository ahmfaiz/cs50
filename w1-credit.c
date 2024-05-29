#include <cs50.h>
#include <math.h>
#include <stdio.h>

#define SMALLEST_CARD 4000000000000
#define LARGEST_CARD 5599999999999999

int main(int argc, char **argv)
{
    long card;

    // Input card number
    do
    {
        card = get_long("Enter Credit card number: ");
    }
    while (card < 0);

    // Check if card number is possible
    if (card < SMALLEST_CARD || card > LARGEST_CARD)
    {
        printf("INVALID\n");
        return 0;
    }

    long temp = card;
    int size = 0;

    // Calculate number of digits in card and store in size
    do
    {
        temp /= 10;
        size++;
    }
    while (temp > 0);

    int double_sum = 0;
    int single_sum = 0;
    int digit;

    // Luhn’s Algorithm
    for (int i = 1; i <= size; i++)
    {
        digit =
            ((card % (long) (pow(10, i))) - (card % (long) (pow(10, i - 1)))) /
            (pow(10, i - 1));
        switch (i % 2)
        {
            case 1:
                single_sum += digit;
                break;

            case 0:
                double_sum += ((2 * digit) % 10) + ((2 * digit) / 10);
                break;

            default:
                perror("ERROR\n");
                break;
        }
    }
    if ((single_sum + double_sum) % 10 != 0)
    {
        printf("INVALID\n");
        return 0;
    }

    // Check card company
    switch ((int) (card / (pow(10, size - 2))))
    {
        case 34:
        case 37:
            if (size == 15)
            {
                printf("AMEX\n");
                return 0;
            }
            else
            {
                printf("INVALID\n");
                return 0;
            }

        case 51:
        case 52:
        case 53:
        case 54:
        case 55:
            if (size == 16)
            {
                printf("MASTERCARD\n");
                return 0;
            }
            else
            {
                printf("INVALID\n");
                return 0;
            }

        case 41:
        case 42:
        case 43:
        case 44:
        case 45:
        case 46:
        case 47:
        case 48:
        case 49:
        case 40:
            if (size == 13 || size == 16)
            {
                printf("VISA\n");
                return 0;
            }
            else
            {
                printf("INVALID\n");
                return 0;
            }

        default:
            printf("INVALID\n");
            return 0;
    }

    return 0;
}

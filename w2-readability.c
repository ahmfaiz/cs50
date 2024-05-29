#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    string text = get_string("Text: ");
    float letters = 0;
    float words = 0;
    float sentences = 0;

    // Counting number of letters, words and sentences
    int i = 0;
    while (i < strlen(text))
    {
        if (isalpha(text[i]))
        {
            letters++;
        }

        else if (isspace(text[i]))
        {
            words++;
        }

        else if ((text[i] == '.') || (text[i] == '!') || (text[i] == '?'))
        {
            sentences++;
            words++;
            i++;
        }

        i++;
    }

    // Calculating grade : Coleman-Liau index
    float index =
        (0.0588 * ((letters / words) * 100)) - (0.296 * ((sentences / words) * 100)) - 15.8;
    int grade = round(index);

    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }

    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }

    else
    {
        printf("Grade %i\n", grade);
    }

    return 0;
}

#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    // Storing points alphabeticaly
    static int points[] = {1, 3, 3, 2,  1, 4, 2, 4, 1, 8, 5, 1, 3,
                           1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

    string move1 = get_string("Player 1: ");
    string move2 = get_string("Player 2: ");

    int score1 = 0;
    int score2 = 0;

    // Calculating score of player 1
    for (int i = 0; i < strlen(move1); i++)
    {
        if (isalpha(move1[i]) > 0)
        {
            score1 += points[(int) (toupper(move1[i]) - (int) 'A')];
        }
    }

    // Calculating score of player 2
    for (int i = 0; i < strlen(move2); i++)
    {
        if (isalpha(move2[i]) > 0)
        {
            score2 += points[(int) (toupper(move2[i]) - (int) 'A')];
        }
    }

    // Result
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score1 < score2)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }

    return 0;
}

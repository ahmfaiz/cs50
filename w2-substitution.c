#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    // Check for only 1 argument (key)
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    int keysize = strlen(argv[1]);
    char key[keysize];

    strcpy(key, argv[1]);

    // Check for key length = 26
    if (keysize != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    // check for key validity
    for (int i = 0; i < 26; i++)
    {
        if (!(strchr(key, 'a' + i) || strchr(key, 'A' + i)))
        {
            printf("Key should contain all letters.\n");
            return 1;
        }
    }

    string plain = get_string("plaintext: ");
    int textsize = strlen(plain);
    char *cipher = malloc(textsize + 1);
    int index;
    int i;
    for (i = 0; i < textsize; i++)
    {
        // if letter
        if ((tolower(plain[i]) >= 97) && (tolower(plain[i]) <= 122))
        {
            // find index of key to be replaced with
            index = tolower(plain[i]) - 'a';

            // Replace if upper or lower letter depending on plaintext
            cipher[i] = isupper(plain[i]) ? toupper(key[index]) : tolower(key[index]);
        }
        // if not a letter
        else
        {
            cipher[i] = plain[i];
        }
    }
    cipher[i] = '\0';

    printf("ciphertext: %s\n", cipher);
    return 0;
}

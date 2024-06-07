// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Number of buckets in hash table
const unsigned int N = 200;

// Number of sub-buckets
const unsigned int M = 50;

// Hash table
node **table[N];

unsigned int size_of_dict = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Calculate hash values
    int hash_value = hash(word);
    int pos1 = hash_value / 100;
    int pos2 = hash_value % 100;

    // If entry not in hash table
    if (table[pos1] == NULL)
    {
        return false;
    }
    // If entry not in sub table
    else if (table[pos1][pos2] == NULL)
    {
        return false;
    }
    // Check for entry in linked list
    else
    {
        node *ptr;
        ptr = table[pos1][pos2];
        do
        {
            if (strcasecmp(ptr->word, word) == 0)
            {
                return true;
            }
            ptr = ptr->next;
        }
        while (ptr != NULL);
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int sum = 0;

    for (int i = 0; word[i] != '\0'; i++)
    {
        sum += tolower(word[i]);
    }

    // Hash for hash table
    int hash1 = sum % N;

    // Hash for sub table
    int hash2 = sum % M;

    // Final hash
    return ((hash1 * 100) + hash2);
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Initializing hash table to NULL
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    // Opening dictionary in read mode
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        printf("Could not open dictionary file.\n");
        return false;
    }

    char buff[LENGTH + 1];
    char c;
    int index = 0;
    node *ptr;
    int hash_value, pos1, pos2;

    // Reading words from dictionary file
    while (fread(&c, sizeof(char), 1, dict))
    {
        if (c != '\n')
        {
            buff[index] = c;
            index++;
        }
        else
        {
            buff[index] = '\0';
            index = 0;

            // Adding word to hash table----------START
            hash_value = hash(buff);

            pos1 = hash_value / 100;
            pos2 = hash_value % 100;

            // If pos1 does not exist in hash table
            if (table[pos1] == NULL)
            {
                table[pos1] = calloc(M, sizeof(node *));
                if (table[pos1] == NULL)
                {
                    printf("Could not create space for hash table.\n");
                    fclose(dict);
                    return false;
                }
                table[pos1][pos2] = calloc(1, sizeof(node));
                if (table[pos1][pos2] == NULL)
                {
                    printf("Could not create space for hash table.\n");
                    fclose(dict);
                    return false;
                }
                strcpy(table[pos1][pos2]->word, buff);
                size_of_dict++;
            }

            // If pos2 does not exist in sub table
            else if (table[pos1][pos2] == NULL)
            {
                table[pos1][pos2] = calloc(1, sizeof(node));
                if (table[pos1][pos2] == NULL)
                {
                    printf("Could not create space for hash table.\n");
                    fclose(dict);
                    return false;
                }
                strcpy(table[pos1][pos2]->word, buff);
                size_of_dict++;
            }

            // If both pos1 and pos2 exist
            else
            {
                ptr = table[pos1][pos2];
                while (ptr->next != NULL)
                {
                    ptr = ptr->next;
                }
                ptr->next = calloc(1, sizeof(node));
                ptr = ptr->next;
                strcpy(ptr->word, buff);
                size_of_dict++;
            }
            // Added word to hash table----------END
        }
    }

    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return size_of_dict;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int pos1 = 0; pos1 < N; pos1++)
    {
        if (table[pos1] == NULL)
        {
            continue;
        }
        for (int pos2 = 0; pos2 < M; pos2++)
        {
            if (table[pos1][pos2] == NULL)
            {
                continue;
            }
            else
            {
                node *ptr = table[pos1][pos2];
                while (ptr != NULL)
                {
                    node *temp = ptr;
                    ptr = ptr->next;
                    free(temp);
                }
            }
        }
        free(table[pos1]);
    }
    return true;
}


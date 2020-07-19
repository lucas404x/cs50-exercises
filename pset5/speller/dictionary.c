// Implements a dictionary's functionality

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>

#include "dictionary.h"


// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Nodes quantity
unsigned int nodes_quantity = 0;

// Number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    unsigned int index = hash(word);
    node *tmp = table[index];

    while (tmp != NULL)
    {
        if (strcasecmp(word, tmp->word) == 0)
        {
            return true;
        }

        tmp = tmp->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int hash = 5381;
    int c;

    for (int i = 0, length = strlen(word); i < length; i++)
    {
        c = tolower(word[i]);
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash % (N - 1);
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    node *value, *tmp;
    char word[48];
    int index;

    FILE *file = fopen(dictionary, "r");

    if (file == NULL)
    {
        return false;
    }

    while (fscanf(file, "%s", word) != EOF)
    {

        // allocating space to a new node
        value = malloc(sizeof(node));

        if (value == NULL)
        {
            return false;
        }

        strcpy(value->word, word);
        value->next = NULL;

        index = hash(word);

        if (table[index] == NULL)
        {
            table[index] = value;
        }

        else
        {
            tmp = table[index];

            // checking if have more nodes
            while (tmp->next != NULL)
            {
                tmp = tmp->next;
            }

            // add node
            tmp->next = value;
        }

        nodes_quantity++;
    }

    fclose(file);

    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return nodes_quantity;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    node *tmp, *aux;

    for (int i = 0; i < N; i++)
    {
        tmp = table[i];

        while (tmp != NULL)
        {
            aux = tmp;
            tmp = tmp->next;
            free(aux);
        }
    }

    nodes_quantity = 0;

    return true;
}

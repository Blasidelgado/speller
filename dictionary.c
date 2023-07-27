// Implements a dictionary's functionality

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Choose number of buckets in hash table
const unsigned int N = 531468; // Bucket for the first three characters

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Create a lowercase copy of word
    int len = strlen(word);
    char lower_word[len + 1];
    for (int i = 0; i < len; i++)
    {
        lower_word[i] = tolower(word[i]);
    }
    lower_word[len] = '\0';
    // Obtain corresponding bucket using hash function
    int bucket = hash(lower_word);
    // Set pointer to traverse linked lists
    node *ptr = table[bucket];
    // Traverse linked lists to find if word exists
    while (ptr != NULL)
    {
        if (strcmp(lower_word, ptr->word) == 0)
        {
            return true;
        }
        else
        {
            ptr = ptr->next;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int hashable_len = strlen(word) > 4 ? 4 : strlen(word); // Max 4 words will be taken into account for creating a key
    return create_key(word, hashable_len);
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Allocate buffer for words
    char buffer[LENGTH + 1];

    // Open file in read mode
    FILE *infile = fopen(dictionary, "r");
    if (infile == NULL)
    {
        printf("Error opening file");
        return false;
    }

    // Store every word of provided dictionary into buffer
    while (fscanf(infile, "%s", buffer) != EOF)
    {
        // Allocate memory for new node
        node *n = malloc(sizeof(node));
        // Check space memory
        if (n == NULL)
        {
            printf("Could not allocate memory for new word");
            return false;
        }
        // Copy from buffer to node
        strcpy(n->word, buffer);
        // Use size to count total words
        size();
        // Hash the word
        int bucket = hash(n->word);
        // Set node to point to corresponding bucket head
        n->next = table[bucket];
        // Add node as bucket head
        table[bucket] = n;
    }
    // Close the dictionary file
    fclose(infile);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    static unsigned int count = 0;
    count++;
    return count - 1; // Not load() calls wont be included
}


// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    node *ptr = NULL, *tmp = NULL;
    // Iterate the hash table
    for (int i = 0; i < N; i++)
    {
        ptr = table[i];
        tmp = table[i];
        // If points to next node, follow
        while (ptr != NULL)
        {
            ptr = ptr->next;
            free(tmp);
            tmp = ptr;
        }
    }
    return true;
}

unsigned int create_key(const char *word, int length)
{
    int x = word[length - 1] - 'a';
    x = x >= 0 ? x : 26; // If letter is an apostrophe, map it to 26

    // Base case
    if (length == 1)
    {
        return x;
    }
    // Recursive case
    else
    {
        return (27 * create_key(word, length - 1)) + x + 27;
    }
}

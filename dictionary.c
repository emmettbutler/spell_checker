/****************************************************************************
 * dictionary.c
 *
 * MPATE-GE 2618
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "dictionary.h"
#include "my_strmap.h"

strMap *map;

/* Returns true if word is in dictionary else false. */
bool check(char *word)
{
    // use convenient hash table lookup
    return sm_get(map, word);
}


/* Loads dict into memory.  Returns true if successful else false. */
bool load(char *dict)
{
    FILE *f = fopen(dict, "r");
    char line[MAX_WORD_LEN + 2]; // add for newline and null char
    map = sm_new(52937); // huge prime number of hash slots

    for(int i = 0; i < MAX_WORD_LEN + 2; i++){
        line[i] = '\0';
    }
    // read words from dictionary and hash them
    if(f != NULL){
        while(fgets(line, sizeof(line), f) != NULL){
            line[strlen(line) - 1] = 0;
            // use convenient hash table insert function
            sm_put(map, line);
        }
        return true;
    } else {
        return false;
    }
}

/* Returns number of words in dictionary if loaded else 0 if not yet loaded. */
int size()
{
    if(map){
        // convenient!
        return sm_get_count(map);
    }
    return 0;
}

/* Delete all dynamically allocated memory if necessary */
void cleanUp()
{
    sm_dealloc(map);
}


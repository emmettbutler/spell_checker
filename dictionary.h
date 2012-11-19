/****************************************************************************
 * dictionary.h
 *
 * MPATE-GE 2618
 * Problem Set 5
 *
 * Declares a dictionary's functionality.
 ***************************************************************************/
#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_

#include <stdbool.h>

// maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
extern const int MAX_WORD_LEN;

/* Returns true if word is in dictionary else false */
bool check(char *word);

/* Loads dictionary into memory.  Returns true if successful else false. */
bool load(char *dict);

/* Returns number of words in dictionary if loaded else 0 if not yet loaded. */
int size();

/* Deletes dynamically allocated memory */
void cleanUp();

#endif // _DICTIONARY_H_

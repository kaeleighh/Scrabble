//
//  Word.h
//

#ifndef Word_h
#define Word_h

#include <stdio.h>
#include "List.h"
#include "Tile.h"


typedef struct _Word_ {
    char    *letters;
    int     mult;
    int     points;
    List    *tiles;
} Word;


/* Public Function Definitions */

Word*   word_init(char *letters, List *tiles);
int     word_GetPoints(Word *word, int values[128]);
void    word_destroy(Word *word);


#endif /* Word_h */

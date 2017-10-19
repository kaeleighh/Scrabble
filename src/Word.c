//
//  Word.c
//

#include "Word.h"


/* Private Function Definitions */

void AddLetterPoints(Word *word, char letter, Tile *tile, int values[128]);


/* Public Functions */

Word* word_init(char *letters, List *tiles) {
    Word *word = NULL;
    word = malloc(sizeof(Word));
    if(word == NULL) {
        return NULL;
    }
    word->letters = letters;
    word->mult = 1;
    word->points = 0;
    word->tiles = tiles;
    return word;
}


int word_GetPoints(Word *word, int values[128]) {
    Node *tile_node = NULL;
    tile_node = word->tiles->head;
    int i = 0;
    do {
        Tile *tile = NULL;
        tile = tile_node->data;
        AddLetterPoints(word, word->letters[i], tile, values);
        i++;
    } while((tile_node = tile_node->next));
    word->points *= word->mult;
    return word->points;
}


void word_destroy(Word *word) {
    free(word->letters);
    word->tiles = NULL;
    free(word);
}


/* Private Functions */

void AddLetterPoints(Word *word, char letter, Tile *tile, int values[128]) {
    int points;
    points = values[letter];
    if(letter != tile->letter) {
        points *= tile->letter_mult;
        word->mult *= tile->word_mult;
    }
    word->points += points;
}

//
//  Move.c
//

#include "Move.h"


/* Private Function Definitions */

int CrosswordIsValid(char* word, char **words);
int MatchCrossword(Move *move, Crosspath *crosspath, char ***words);
int GetCrosswordPoints(Move *move, int values[128]);


/* Public Functions */

Move* move_init(char *letters, int used, List *tiles) {
    Move *move = NULL;
    move = malloc(sizeof(Move));
    if(move == NULL) {
        return NULL;
    }
    move->crosswords = NULL;
    move->crosswords = list_init();
    move->points = 0;
    move->tiles = tiles;
    move->used = used;
    move->word = word_init(letters, tiles);
    return move;
}


int move_MatchCrosswords(Move *move, List *crosspaths, char ***words) {
    Node *crosspath_node = NULL;
    crosspath_node = crosspaths->head;
    while(crosspath_node) {
        Crosspath *crosspath = NULL;
        crosspath = crosspath_node->data;
        if(MatchCrossword(move, crosspath, words)) {
            crosspath_node = crosspath_node->next;
        }
        else {
            move_destroy(move);
            return 0;
        }
    }
    return 1;
}


int move_GetPoints(Move *move, int values[128]) {
    move->points = word_GetPoints(move->word, values);
    move->points += move->used == 7 ? 40 : move->used == 8 ? 50 : 0;
    move->points += GetCrosswordPoints(move, values);
    return move->points;
}


void move_destroy(Move *move) {
    Node *node_crossword = NULL;
    node_crossword = move->crosswords->head;
    while(node_crossword) {
        Word *crossword = node_crossword->data;
        word_destroy(crossword);
        node_crossword = node_crossword->next;
    }
    list_destroy(move->crosswords);
    move->tiles = NULL;
    word_destroy(move->word);
    free(move);
}


/* Private Functions */

int MatchCrossword(Move *move, Crosspath *crosspath, char ***words) {
    char *letters = NULL;
    letters = malloc(sizeof(char) * (strlen(crosspath->letters) + 1));
    if(letters == NULL) {
        exit(-1);
    }
    strcpy(letters, crosspath->letters);
    char intersection = move->word->letters[crosspath->intersection];
    memcpy(strchr(letters, '_'), &intersection, sizeof(char));
    if(CrosswordIsValid(letters, words[strlen(letters)])) {
        Word *word = NULL;
        word = word_init(letters, crosspath->tiles);
        list_Push(move->crosswords, word);
        return 1;
    }
    else {
        free(letters);
        return 0;
    }
}


int CrosswordIsValid(char* word, char **words) {
    int i = 0;
    while(words[i]) {
        if(strcasecmp(word, words[i]) == 0) {
            return 1;
        }
        i++;
    }
    return 0;
}


int GetCrosswordPoints(Move *move, int values[128]) {
    int points = 0;
    Node *crossword_node = move->crosswords->head;
    while(crossword_node) {
        Word *crossword = crossword_node->data;
        points += word_GetPoints(crossword, values);
        crossword_node = crossword_node->next;
    }
    return points;
}

//
//  Path.c
//

#include "Path.h"


/* Private Function Definitions*/

int AddLettersToWord(char *word, List *tiles, char *match, char *available);
int AddLetterToWord(char *word, int index, Tile *tile, char needed, char *available);
char* GetAvailableLetters(char *hand);
char* MatchWord(List *tiles, char *match, char *hand);
void RemoveFromString(char *str, char *chr);


/* Public Functions */

Path* path_init() {
    Path *path;
    path = malloc(sizeof(Path));
    if(path == NULL) {
        return NULL;
    }
    path->crosspaths = list_init();
    path->moves = list_init();
    path->tiles = list_init();
    path->used = 0;
    return path;
}


int path_AddTile(Path *path, Tile *tile, char *hand) {
    path->used += tile->letter == '_';
    if(path->used > strlen(hand)) {
        return 0;
    }
    list_Push(path->tiles, tile);
    return 1;
}


void path_Copy(Path *dst, Path *src) {
    dst->dx = src->dx;
    dst->dy = src->dy;
    dst->used = src->used;
    list_Copy(dst->tiles, src->tiles);
}


void path_FindMoves(Path *path, char ***words, char *hand) {
    int size = path->tiles->size;
    int i = 0;
    while(words[size][i]) {
        char *word = NULL;
        if((word = MatchWord(path->tiles, words[size][i], hand))) {
            Move *move = NULL;
            move = move_init(word, path->used, path->tiles);
            if(move_MatchCrosswords(move, path->crosspaths, words)) {
                list_Push(path->moves, move);
            }
        }
        i++;
    }
}


void path_destroy(Path *path) {
    Node *move_node = NULL;
    move_node = path->moves->head;
    while(move_node) {
        Move *move = move_node->data;
        move_destroy(move);
        move_node = move_node->next;
    }
    list_destroy(path->moves);
    path->moves = NULL;
    Node *crosspath_node = NULL;
    crosspath_node = path->crosspaths->head;
    while(crosspath_node) {
        Crosspath *crosspath = crosspath_node->data;
        crosspath_destroy(crosspath);
        crosspath_node = crosspath_node->next;
    }
    list_destroy(path->crosspaths);
    path->crosspaths = NULL;
    list_destroy(path->tiles);
    path->tiles = NULL;
    free(path);
}


/* Private Functions */

char* MatchWord(List *tiles, char *match, char *hand) {
    char *word = NULL;
    word = malloc(sizeof(char) * (tiles->size + 1));
    if(word == NULL) {
        exit(-1);
    }
    char *available = NULL;
    available = GetAvailableLetters(hand);
    if(AddLettersToWord(word, tiles, match, available)) {
        free(available);
        return word;
    }
    else {
        free(available);
        free(word);
        return NULL;
    }
}


char* GetAvailableLetters(char *hand) {
    char *available = NULL;
    available = malloc((strlen(hand) + 1) * sizeof(char));
    if(available == NULL) {
        return NULL;
    }
    memcpy(available, hand, (strlen(hand) + 1) * sizeof(char));
    available[strlen(hand)] = '\0';
    return available;
}


int AddLettersToWord(char *word, List *tiles, char *match, char *available) {
    int i =0;
    Node *tile_node = NULL;
    tile_node = tiles->head;
    do {
        Tile *tile = NULL;
        tile = tile_node->data;
        if(!AddLetterToWord(word, i, tile, match[i], available)) {
            return 0;
        }
        i++;
    } while((tile_node = tile_node->next));
    word[i] = '\0';
    return 1;
}


int AddLetterToWord(char *word, int index, Tile *tile, char needed, char *available) {
    if(tile->letter == needed) {
        word[index] = needed;
        return 1;
    }
    else if(tile->letter == '_') {
        char *use = strchr(available, needed);
        use = use ? use : strchr(available, '?');
        if(use) {
            word[index] = *use == '?' ? tolower(needed) : needed;
            RemoveFromString(available, use);
            return 1;
        }
    }
    return 0;
}


void RemoveFromString(char *str, char *chr) {
    if(!chr) {
        return;
    }
    int index = (int)(&chr[0] - &str[0]) / sizeof(char);
    memcpy(&str[index], &str[index + 1], strlen(str) - index);
}

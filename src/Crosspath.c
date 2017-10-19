//
//  Crosspath.c
//

#include "Crosspath.h"


/* Public Functions */

Crosspath* crosspath_init(int intersection) {
    Crosspath *crosspath;
    crosspath = malloc(sizeof(Crosspath));
    if(crosspath == NULL) {
        return NULL;
    }
    crosspath->intersection = intersection;
    crosspath->letters = malloc(sizeof(char) * 16);
    crosspath->tiles = list_init();
    return crosspath;
}


void crosspath_GetLetters(Crosspath *crosspath) {
    Node *node = crosspath->tiles->head;
    int i = 0;
    do {
        Tile *tile = node->data;
        crosspath->letters[i] = tile->letter;
        i++;
    } while((node = node->next));
    crosspath->letters[i] = '\0';
}


void crosspath_destroy(Crosspath *crosspath) {
    free(crosspath->letters);
    list_destroy(crosspath->tiles);
    free(crosspath);
}

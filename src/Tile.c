//
//  Tile.c
//

#include "Tile.h"


/* Public Functions */

Tile* tile_init(int x, int y) {
    Tile *tile = NULL;
    tile = malloc(sizeof(Tile));
    if(tile == NULL) {
        return NULL;
    }
    tile->x = x;
    tile->y = y;
    tile->letter = '_';
    tile->letter_mult = 1;
    tile->word_mult = 1;
    return tile;
}

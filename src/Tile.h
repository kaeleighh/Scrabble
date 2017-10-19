//
//  Tile.h
//

#ifndef Tile_h
#define Tile_h

#include <stdio.h>
#include <stdlib.h>


typedef struct _Tile_ {
    int     x;
    int     y;
    char    letter;
    int     letter_mult;
    int     word_mult;
} Tile;


/* Public Function Definitions */

Tile*   tile_init(int x, int y);

#endif /* Tile_h */

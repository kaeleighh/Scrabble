//
//  Board.h
//

#ifndef Board_h
#define Board_h

#include <stdio.h>
#include "List.h"
#include "Path.h"
#include "Tile.h"


typedef struct _Board_ {
    Tile*   tiles[15][15];
    List*   paths;
} Board;


/* Public Function Definitions */

Board*  board_init();
void    board_FindPaths(Board *board, char *hand, int dx);
void    board_destroy(Board *board);


#endif /* Board_h */

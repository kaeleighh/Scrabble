//
//  Path.h
//

#ifndef Path_h
#define Path_h

#include <stdio.h>
#include <ctype.h>
#include "Crosspath.h"
#include "List.h"
#include "Move.h"
#include "Tile.h"


typedef struct _Path_ {
    List    *crosspaths;
    int     dx;
    int     dy;
    List    *moves;
    List    *tiles;
    int     used;
} Path;


/* Public Function Definitions */

Path*   path_init();
int     path_AddTile(Path *path, Tile *tile, char *hand);
void    path_Copy(Path *dst, Path *src);
void    path_FindMoves(Path *path, char ***words, char *hand);
void    path_destroy(Path *path);


#endif /* Path_h */

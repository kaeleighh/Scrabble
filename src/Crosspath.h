//
//  Crosspath.h
//

#ifndef Crosspath_h
#define Crosspath_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "List.h"
#include "Tile.h"


typedef struct _Crosspath_ {
    int     intersection;
    char    *letters;
    int     size;
    List    *tiles;
} Crosspath;


/* Public Function Definitions */

Crosspath*  crosspath_init(int intersection);
void        crosspath_GetLetters(Crosspath *crosspath);
void        crosspath_destroy(Crosspath *crosspath);


#endif /* Crosspath_h */

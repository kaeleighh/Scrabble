//
//  Board.c
//

#include "Board.h"


/* Private Function Definitions */

void    AddCrosspath(Board *board, Path *path, Tile *tile, int intersection);
void    AddPath(Board *board, Path *path);
int     CrosspathHasTiles(Board *board, Path *path, Tile *tile);
void    FindCrosspaths(Board *board, Path *path);
void    FindPathsFromTile(Board *board, Tile *tile, char *hand, int dx);
int     OnBoard(int x, int y);
int     PathIsConnected(Board *board, Path *path);
int     PathIsValid(Board *board, Path *path);
void    ReadLetterMultipliers(Board *board);
void    ReadTiles(Board *board);
void    ReadWordMultipliers(Board *board);
int     TileIsBlank(Board *board, int x, int y);
int     TileIsConnected(Board *board, int x, int y);
int     TileIsNotBlank(Board *board, int x, int y);


/*  Public Functions  */

Board* board_init() {
    Board *board;
    board = malloc(sizeof(Board));
    if(board == NULL) {
        return NULL;
    }
    for(int x=0; x<15; x++) {
        for(int y=0; y<15; y++) {
            Tile *tile = NULL;
            tile = tile_init(x, y);
            board->tiles[x][y] = tile;
        }
    }
    board->paths = NULL;
    board->paths = list_init();
    ReadTiles(board);
    ReadLetterMultipliers(board);
    ReadWordMultipliers(board);
    return board;
}


void board_FindPaths(Board *board, char *hand, int dx) {
    int dy = !dx;
    for(int x=0; x<15; x++) {
        for(int y=0; y<15; y++) {
            FindPathsFromTile(board, board->tiles[x][y], hand, dx);
        }
    }
    if(dx) {
        board_FindPaths(board, hand, dy);
    }
}


void board_reset(Board *board) {
    Node *path_node = NULL;
    path_node = board->paths->head;
    while(path_node) {
        Path *path = path_node->data;
        path_destroy(path);
        path_node = path_node->next;
    }
    list_destroy(board->paths);
    board->paths = NULL;
}


void board_destroy(Board *board) {
    board_reset(board);
    for(int x=0; x<15; x++) {
        for(int y=0; y<15; y++) {
            free(board->tiles[x][y]);
        }
    }
    free(board);
}


/*  Private Functions  */

void FindPathsFromTile(Board *board, Tile *start, char *letters, int dx) {
    int x = start->x;
    int y = start->y;
    int dy = !dx;
    if(TileIsNotBlank(board, x - dx, y - dy)) {
        return;
    }
    Path *path = NULL;
    path = path_init();
    path->dx = dx;
    path->dy = dy;
    while(OnBoard(x, y) && path_AddTile(path, board->tiles[x][y], letters)) {
        if(PathIsValid(board, path)) {
            AddPath(board, path);
        }
        x += dx;
        y += dy;
    }
    path_destroy(path);
}


int PathIsValid(Board *board, Path *path) {
    if(PathIsConnected(board, path) && path->used > 0) {
        Tile *last = path->tiles->tail->data;
        int next_x = last->x + path->dx;
        int next_y = last->y + path->dy;
        if(!OnBoard(next_x, next_y)) {
            return 1;
        }
        return TileIsBlank(board, next_x, next_y);
    }
    return 0;
}


int PathIsConnected(Board *board, Path *path) {
    Node *tile_node = path->tiles->head;
    do {
        Tile *tile = tile_node->data;
        if(TileIsConnected(board, tile->x, tile->y)) {
            return 1;
        }
    } while((tile_node = tile_node->next));
    return 0;
}


void AddPath(Board *board, Path *source) {
    Path *path = NULL;
    path = path_init();
    path_Copy(path, source);
    FindCrosspaths(board, path);
    list_Push(board->paths, path);
}


void FindCrosspaths(Board *board, Path *path) {
    int intersection = 0;
    Node *tile_node = path->tiles->head;
    do {
        Tile *tile = NULL;
        tile = tile_node->data;
        if(CrosspathHasTiles(board, path, tile)) {
            AddCrosspath(board, path, tile, intersection);
        }
        intersection++;
    } while((tile_node = tile_node->next));
}


int CrosspathHasTiles(Board *board, Path *path, Tile *tile) {
    if(TileIsNotBlank(board, tile->x, tile->y)) {
        return 0;
    }
    if((!OnBoard(tile->x + !path->dx, tile->y + !path->dy) ||
        TileIsBlank(board, tile->x + !path->dx, tile->y + !path->dy)) &&
       (!OnBoard(tile->x - !path->dx, tile->y - !path->dy) ||
        TileIsBlank(board, tile->x - !path->dx, tile->y - !path->dy))) {
        return 0;
    }
    return 1;
}


void AddCrosspath(Board *board, Path *path, Tile *tile, int intersection) {
    int x = tile->x;
    int y = tile->y;
    int dx = !path->dx;
    int dy = !path->dy;
    Crosspath *crosspath = NULL;
    crosspath = crosspath_init(intersection);
    do {
        list_Push(crosspath->tiles, board->tiles[x][y]);
    } while(TileIsNotBlank(board, (x += dx), (y += dy)));
    x = tile->x;
    y = tile->y;
    while(TileIsNotBlank(board, (x -= dx), (y -= dy))) {
        list_Insert(crosspath->tiles, board->tiles[x][y]);
    }
    crosspath_GetLetters(crosspath);
    if(strchr(crosspath->letters, '_')) {
        list_Push(path->crosspaths, crosspath);
    }
    else {
        crosspath_destroy(crosspath);
    }
}


int OnBoard(int x, int y) {
    return x > -1 && x < 15 && y > -1 && y < 15;
}


int TileIsBlank(Board *board, int x, int y) {
    return OnBoard(x, y) && board->tiles[x][y]->letter == '_';
}


int TileIsNotBlank(Board *board, int x, int y) {
    return OnBoard(x, y) && board->tiles[x][y]->letter != '_';
}


int TileIsConnected(Board *board, int x, int y) {
    if (TileIsNotBlank(board, x - 1, y) || TileIsNotBlank(board, x + 1, y) ||
        TileIsNotBlank(board, x, y - 1) || TileIsNotBlank(board, x, y + 1)) {
        return 1;
    }
    else if (x == 7 && y == 7) {
        return 1;
    }
    return 0;
}


void ReadLetterMultipliers(Board *board) {
    FILE *file = NULL;
    if((file = fopen("config/letter_multipliers.txt", "r")) == NULL) {
        return;
    }
    char *buff = NULL;
    if((buff = malloc(17 * sizeof(char))) == NULL) {
        return;
    }
    int y = 0;
    while(fgets(buff, 17, file)) {
        for(int x=0; x<15; x++) {
            board->tiles[x][y]->letter_mult = buff[x] - 48;
        }
        y++;
    }
    fclose(file);
}


void ReadTiles(Board *board) {
    int y = 0;
    FILE *file = NULL;
    if((file = fopen("config/board.txt", "r")) == NULL) {
        return;
    }
    char *buff = NULL;
    if((buff = malloc(17 * sizeof(char))) == NULL) {
        return;
    }
    while(fgets(buff, 17, file)) {
        for(int x=0; x<15; x++) {
            board->tiles[x][y]->letter = buff[x];
        }
        y++;
    }
    fclose(file);
}


void ReadWordMultipliers(Board *board) {
    int y = 0;
    FILE *file = NULL;
    if((file = fopen("config/word_multipliers.txt", "r")) == NULL) {
        return;
    }
    char *buff = NULL;
    if((buff = malloc(17 * sizeof(char))) == NULL) {
        return;
    }
    while(fgets(buff, 17, file)) {
        for(int x=0; x<15; x++) {
            board->tiles[x][y]->word_mult = buff[x] - 48;
        }
        y++;
    }
    fclose(file);
}

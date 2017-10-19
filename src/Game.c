//
//  Game.c
//

#include "Game.h"


/* Private Function Definitions */

void    Play(char ***words, int values[128]);
Move*   GetBestMove(Board *board, char *hand, char ***words, int values[128]);
char*   GetHand();
void    PlayTurn(Board *board, char *hand, char ***words, int values[128]);
void    PrintResult(Move *move, double time_used);
char*** ReadDictionary();


/* Public Functions */

void game_Setup() {
    char ***words = NULL;
    words = ReadDictionary();
    int values[128] = {0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,4,4,2,1,
        5,2,5,1,8,6,1,4,1,1,4,12,1,1,2,1,5,5,8,5,
        12,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    Play(words, values);
}


/* Private Functions */

void Play(char ***words, int values[128]) {
    char *hand = NULL;
    hand = GetHand();
    if(strlen(hand) > 0) {
        Board *board = NULL;
        board = board_init();
        PlayTurn(board, hand, words, values);
        board_destroy(board);
        free(hand);
        hand = NULL;
        Play(words, values);
    }
    else {
        free(hand);
        hand = NULL;
    }
}


void PlayTurn(Board *board, char *hand, char ***words, int values[128]) {
    clock_t start, end;
    double time_used;
    start = clock();
    Move *best_move = NULL;
    best_move = GetBestMove(board, hand, words, values);
    end = clock();
    time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    PrintResult(best_move, time_used);
}


Move* GetBestMove(Board *board, char *hand, char ***words, int values[128]) {
    Move *best_move = NULL;
    board_FindPaths(board, hand, 1);
    Node *path_node = NULL;
    path_node = board->paths->head;
    while(path_node) {
        Path *path = path_node->data;
        path_FindMoves(path, words, hand);
        Node *move_node = NULL;
        move_node = path->moves->head;
        while(move_node) {
            Move *move = move_node->data;
            move_GetPoints(move, values);
            if(best_move) {
                best_move = best_move->points > move->points ? best_move : move;
            }
            else {
                best_move = move;
            }
            move_node = move_node->next;
        }
        path_node = path_node->next;
    }
    return best_move;
}


char* GetHand() {
    printf("%s", "\nEnter your letters: \n");
    char* hand = NULL;
    hand = malloc(sizeof(char) * 10);
    fgets(hand, 10, stdin);
    for(int i=0; i<10; i++) {
        if(hand[i] == '\n') {
            hand[i] = '\0';
        }
    }
    if(strlen(hand) > 8) {
        return GetHand();
    }
    return hand;
}


char*** ReadDictionary() {
    FILE *file = NULL;
    if((file = fopen("config/words.txt", "r")) == NULL) {
        exit(-1);
    }
    fseek(file, 0L, SEEK_END);
    long size = ftell(file);
    rewind(file);
    char *buff = NULL;
    if((buff = malloc(size * sizeof(char))) == NULL) {
        exit(-1);
    }
    fread(buff, sizeof(char), (int) size, file);
    int num_lines = 0;
    int i = 0;
    while(i < size) {
        if(buff[i] == '\n') {
            num_lines++;
        }
        i++;
    }
    rewind(file);
    char ***words = NULL;
    words = malloc(sizeof(char**) * 16);
    if(words == NULL) {
        exit(-1);
    }
    for(int i=0; i<16; i++) {
        char **words_subset = NULL;
        words_subset = malloc(sizeof(char*) * 30000);
        if(words_subset == NULL) {
            exit(-1);
        }
        words[i] = words_subset;
    }
    int word_counters[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int word_length;
    for(int j=0; j<num_lines; j++) {
        char *word = malloc(sizeof(char) * 16);
        fgets(word, sizeof(char) * 16, file);
        char *end_of_line;
        if ((end_of_line = strchr(word, '\n')) != NULL) {
            *end_of_line = '\0';
        }
        word_length = (int)strlen(word);
        words[word_length][word_counters[word_length]] = word;
        word_counters[word_length]++;
    }
    fclose(file);
    return words;
}


void PrintResult(Move *move, double time_used) {
    printf("%s", "\nBest move (found in ");
    printf("%f", time_used);
    printf("%s", " seconds):");
    if(move) {
        Tile *first = move->tiles->head->data;
        Tile *last = move->tiles->tail->data;
        printf("%s", "\n");
        printf("%i", first->x); printf("%s", ", "); printf("%i", first->y);
        printf("%s", " to ");
        printf("%i", last->x); printf("%s", ", "); printf("%i", last->y);
        printf("%s", "\n"); printf("%s", move->word->letters);
        printf("%s", "\n Points: "); printf("%i", move->points);
        printf("%s", "\n");
    }
    else {
        printf("%s", "\nNo moves.");
    }
}

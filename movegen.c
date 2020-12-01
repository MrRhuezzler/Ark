#include "ark.h"

/*

Generate Moves(board, list)
    Iterate over the pieces
        -> Slider loop in all posible directions and add move
            -> Add Move list->moves[list->count]; list->count++;

*/

// Adding a normal move
void addQuietMove(const S_BOARD *board, int move, S_MOVELIST *list){
    list->moves[list->count].move = move;
    list->moves[list->count].score = 0;
    list->count++;
}

// Adding a Capture move
void addCaptureMove(const S_BOARD *board, int move, S_MOVELIST *list){
    list->moves[list->count].move = move;
    list->moves[list->count].score = 0;
    list->count++;
}

// Adding a EnPass move
void addEnPassMove(const S_BOARD *board, int move, S_MOVELIST *list){
    list->moves[list->count].move = move;
    list->moves[list->count].score = 0;
    list->count++;
}




void GenerateAllMoves(const S_BOARD *board, S_MOVELIST *list){
    list->count = 0;
}





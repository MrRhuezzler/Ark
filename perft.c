#include "ark.h"

int total_nodes = 0;

void perft(int depth, S_BOARD *board){

    ASSERT(CheckBoard(board));
    
    if(depth == 0){
        total_nodes++;
        return;
    }

    S_MOVELIST list[1];
    GenerateAllMoves(board, list);

    for(int i = 0; i < list->count; i++){
        
        if(!MakeMove(board, list->moves[i].move)) continue;
        perft(depth - 1, board);
        TakeMove(board);
    }

    return;
}

void perftTest(int depth, S_BOARD *board){

    printf("Starting Searching for depth : %d\n", depth);

    S_MOVELIST list[1];

    GenerateAllMoves(board, list);

    long cummulative_nodes;

    for(int i = 0; i < list->count; i++){

        if(!MakeMove(board, list->moves[i].move)){
            continue;
        }

        cummulative_nodes = total_nodes;
        perft(depth - 1, board);
        TakeMove(board);
        long old = total_nodes - cummulative_nodes;
        printf("%2d) %s : %ld\n", i + 1, PrMove(list->moves[i].move), old);
    }

    printf("Total Moves Searched... %ld\n", total_nodes);
}

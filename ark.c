#include "ark.h"

#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define E4_FEN "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"
#define C5_FEN "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"
#define CUSTOM_FEN "r1bqkb1r/pp3p1p/2np1np1/2p1p3/8/1P1P1NP1/PBPK1PBP/RN1Q3R w kq - 0 9"
#define FEN_4 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
#define MOVEGEN_TEST_FEN "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
#define C_FEN "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"

int main(){

    Init();

    S_BOARD board[1];

    parseFen(START_FEN, board);
    printBoard(board);

    S_MOVELIST list[1];

    GenerateAllMoves(board, list);

    PrintMoveList(list);

    getchar();

    int move;

    for(int i = 0 ; i < list->count; i++){
        move = list->moves[i].move;

        if(!MakeMove(board, move)){
            continue;
        }

        printf("MOVE : %s\n", PrMove(move));
        printBoard(board);

        TakeMove(board);

        printf("TAKEN : %s\n", PrMove(move));
        printBoard(board);

        getchar();

    }


    return 0;
}
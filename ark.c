#include "ark.h"

#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define E4_FEN "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"
#define C5_FEN "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"
#define CUSTOM_FEN "r1bqkb1r/pp3p1p/2np1np1/2p1p3/8/1P1P1NP1/PBPK1PBP/RN1Q3R w kq - 0 9"
#define FEN_4 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"

#define C_FEN "5p1q/8/5p1p/8/8/P1P5/8/Q1P5 w - - 0 1"

int main(){

    Init();

    S_BOARD board;

    parseFen(C_FEN, &board);
    printBoard(&board);


    int move = 0;
    int from = A2, to = H7;
    int cap = wR, pro = bB;
    move = from | (to << 7) | (cap << 14) | (pro << 20);
    printf("%d %X\n", move, move);

    printf("From Sq : %s\n", PrSq(from));
    printf("To Sq : %s\n", PrSq(to));
    printf("Move : %s", PrMove(move));


    // printf("FROM : %d TO : %d CAP : %d PRO : %d", FROSQ(move), TOSQ(move), CAPTURED(move), PROMOTION(move));

    return 0;
}
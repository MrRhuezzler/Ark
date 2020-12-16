#include "ark.h"

#define FEN "k2qr3/5PPP/8/8/8/8/8/KR3Q2 w - - 0 1"
#define MATEIN3 "2rr3k/pp3pp1/1nnqbN1p/3pN3/2pP4/2P3Q1/PPB4P/R4RK1 w - -"
#define MATEIN5 "2q1nk1r/4Rp2/1ppp1P2/6Pp/3p1B2/3P3P/PPP1Q3/6K1 w - - 0 1"
#define MOVE_ORDERING "r1b1k2r/ppppnppp/2n2q2/2b5/3NP3/2P1B3/PP3PPP/RN1QKB1R w - - 0 1"

int main(){

    Init();
    /*

    S_BOARD board[1];
    InitPvTable(board->pvTable);
    S_SEARCHINFO info[1];

    parseFen(MOVE_ORDERING, board);

    char input[6];
    int move;

    while (TRUE)
    {
        printBoard(board);
        printf("Enter a move > ");
        fgets(input, 6, stdin);

        if(input[0] == 'q'){
            break;
        }else if(input[0] == 't'){
            TakeMove(board);
        }else if(input[0] == 's'){
            info->depth = 5;
            info->starttime = getMillis();
            info->stoptime = getMillis() + 200000;
            SearchPosition(board, info);
        }else{
            move = ParseInputMove(input, board);
            if(move != NOMOVE){
                StorePvTable(board, move);
                MakeMove(board, move);
            }else{
                printf("Move not parsed correctly!\n");
            }
        }

        fflush(stdin);
    }
    free(board->pvTable->pTable);
    */

    UCILoop();

    return 0;
}
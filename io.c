#include "ark.h"

void SqAt(const int side, const S_BOARD *board){
    for(int rank = RANK_8; rank >= RANK_1; rank--){
        for(int file = FILE_A; file <= FILE_H; file++){
            int sq = FR2SQ(file, rank);
            printf("%5c", isSqAttacked(sq, side, board) ? 'X' : '.');
        }
        printf("\n\n");
    }
    printf("\n\n");
}


char * PrSq(const int sq){
    static char SqStr[3];
    sprintf(SqStr, "%c%c", ('a' + FilesBrd[sq]), ('1' + RanksBrd[sq]));
    return SqStr;
}

char * PrMove(const int move){
    static char MvStr[5];

    int from_f = FilesBrd[FROSQ(move)];
    int from_r = RanksBrd[FROSQ(move)];
    int to_f = FilesBrd[TOSQ(move)];
    int to_r = RanksBrd[TOSQ(move)];

    int promotedPiece = PROMOTION(move);
    char promotedPieceChar;

    if(promotedPiece){
        promotedPieceChar = 'q';
        if(IsKn(promotedPiece)) promotedPieceChar = 'k';
        else if(IsRQ(promotedPiece) && !IsBQ(promotedPiece)) promotedPieceChar = 'r';
        else if(IsBQ(promotedPiece) && !IsRQ(promotedPiece)) promotedPieceChar = 'b';
        sprintf(MvStr, "%c%c%c%c%c", 'a' + from_f, '1' + from_r, 'a' + to_f, '1' + to_r, promotedPieceChar);
    }else sprintf(MvStr, "%c%c%c%c", 'a' + from_f, '1' + from_r, 'a' + to_f, '1' + to_r);

    return MvStr;
}


void PrintMoveList(const S_MOVELIST *list){

    printf("Movelist\n");
    for(int i =  0; i < list->count; i++){
        printf("%d) Move : %s, Score : %d\n", i + 1, PrMove(list->moves[i].move), list->moves[i].score);
    }
    printf("Total Number of Moves : %d\n", list->count);
}



int ParseInputMove(char *input, S_BOARD *board){

    if(!(input[1] <= '8' && input[1] >= '1')) return NOMOVE;
    if(!(input[3] <= '8' && input[3] >= '1')) return NOMOVE;
    if(!(input[0] <= 'h' && input[0] >= 'a')) return NOMOVE;
    if(!(input[2] <= 'h' && input[2] >= 'a')) return NOMOVE;

    int from = FR2SQ(input[0] - 'a', input[1] - '1');
    int to = FR2SQ(input[2] - 'a', input[3] - '1');

    printf("Parsed Input : %s\n", input);

    ASSERT(SqOnBoard(from) && SqOnBoard(to));

    S_MOVELIST list[1];
    GenerateAllMoves(board, list);
    int promo = EMPTY, move;

    for(int moveNum = 0; moveNum < list->count; moveNum++){
        move = list->moves[moveNum].move;
        if(FROSQ(move) == from && TOSQ(move) == to){
            promo = PROMOTION(move);
            if(promo != EMPTY){
                if(!IsRQ(promo) && IsBQ(promo) && input[4] == 'b'){
                    return move;
                }else if(IsRQ(promo) && !IsBQ(promo) && input[4] == 'r'){
                    return move;
                }else if(IsKn(promo) && input[4] == 'n'){
                    return move;
                }else if(IsRQ(promo) && IsBQ(promo) && input[4] == 'q'){
                    return move;
                }

                continue;
            }

            return move;
        }
    }

    return NOMOVE;

}
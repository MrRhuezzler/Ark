#include "ark.h"

const int KnDir[] = {+8, +12, +19, +21, -8, -12, -19, -21};
const int RkDir[] = {-1, -10, 1, 10};
const int BiDir[] = {-9, -11, 11, 9};
const int KiDir[] = {-9, -11, 11, 9, -1, -10, 1, 10};

int isSqAttacked(const int sq, const int side, const S_BOARD *board){

    ASSERT(SqOnBoard(sq));
    ASSERT(SideValid(side));
    ASSERT(CheckBoard(board));

    // Pawns
    if(side == WHITE){
        if(board->pieces[sq-9] == wP || board->pieces[sq-11] == wP)
            return TRUE;
    }else{
        if(board->pieces[sq+9] == bP || board->pieces[sq+11] == bP)
            return TRUE;
    }

    // Knight
    for(int i =  0; i < 8; i++){
        int piece = board->pieces[sq + KnDir[i]];
        if(IsKn(piece) && PieceCol[piece] == side){
            return TRUE;
        }
    }

    // King
    for(int i = 0; i < 8; i++){
        int piece = board->pieces[sq + KiDir[i]];
        if(IsKi(piece) && PieceCol[piece] == side){
            return TRUE;
        }
    }

    // Rooks and Queens
    for(int i = 0; i < 4; i++){
        int t_sq = sq;
        t_sq += RkDir[i];
        int piece = board->pieces[t_sq];
        while(piece != OFF_BOARD){
            if(piece != EMPTY){
                if(IsRQ(piece) && PieceCol[piece] == side){
                    return TRUE;
                }
                break;
            }
            t_sq += RkDir[i];
            piece = board->pieces[t_sq];
        }
    }


    // Bishop and Queens
    for(int i = 0; i < 4; i++){
        int t_sq = sq;
        t_sq += BiDir[i];
        int piece = board->pieces[t_sq];
        while(piece != OFF_BOARD){
            if(piece != EMPTY){
                if(IsBQ(piece) && PieceCol[piece] == side){
                    return TRUE;
                }
                break;
            }
            t_sq += BiDir[i];
            piece = board->pieces[t_sq];
        }
    }


    return FALSE;
}
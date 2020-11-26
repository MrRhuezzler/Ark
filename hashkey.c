#include "ark.h"

// This Function gives the posKey of the current board
U64 generatePosKey(const S_BOARD *board){

    U64 finalKey = 0;

    // Piece Position Information
    for(int sq = 0; sq < BRD_SQ_NUM; sq++){
        int pieceData = board->pieces[sq];
        if(pieceData != NO_SQ && pieceData != EMPTY && pieceData != OFF_BOARD){
            ASSERT(pieceData >= wP && pieceData <= bK);
            finalKey ^= pieceKey[pieceData][sq];
        }
    }

    // Side Information
    if(board->side == WHITE){
        finalKey ^= sideKey;
    }

    // enPass Information
    if(board->enPass != NO_SQ){
        ASSERT(board->enPass >= 0 && board->enPass < BRD_SQ_NUM);
        finalKey ^= pieceKey[EMPTY][board->enPass];
    }

    // Castle Information
    ASSERT(board->castlePerm >= 0 && board->castlePerm <= 15);
    finalKey ^= castlePemKey[board->castlePerm];


    return finalKey;

}
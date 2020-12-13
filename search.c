#include "ark.h"

int IsRepetition(const S_BOARD *board){
    
    
    for(int his = board->hisply - board->fiftyMoves; his < board->hisply - 1; his++){

        ASSERT(his >= 0 && his < MAX_GAME_MOVES);

        if(board->history[his].posKey == board->posKey){
            return TRUE;
        }
    }

    return FALSE;
}

void SearchPosition(S_BOARD *board){
    
}
#include "ark.h"


int GetPvLine(S_BOARD *board, int depth){


    ASSERT(depth < MAX_DEPTH);

    int move = ProbePvTable(board);
    int count = 0;

    while(move != NOMOVE && count < depth){
            
        ASSERT(count < MAX_DEPTH);

        if(MoveExists(board, move)){
            MakeMove(board, move);
            board->pvArray[count++] = move;
        }else{
            break;
        }
        move = ProbePvTable(board);
    }

    while(board->ply > 0){
        TakeMove(board);
    }

    return count;

}




const int pvSize = 0x100000 * 2; // 2MB of space

void ClearTable(S_PVTABLE *table){

    S_PVENTRY* pventry;

    for(pventry = table->pTable; pventry < table->pTable + table->num_entires; pventry++){
        pventry->move = NOMOVE;
        pventry->poskey = 0ULL;
    }

}


void InitPvTable(S_PVTABLE *table){

    table->pTable = NULL;
    int size = (pvSize / sizeof(S_PVENTRY)) - 2;
    table->num_entires = size;
    table->pTable = (S_PVENTRY *) malloc(table->num_entires * sizeof(S_PVENTRY));
    ClearTable(table);

    printf("PvTable Initialized with %d entries...\n", table->num_entires);
}

void StorePvTable(S_BOARD *board, int move){

    int index = board->posKey % board->pvTable->num_entires;
    board->pvTable->pTable[index].poskey = board->posKey;
    board->pvTable->pTable[index].move = move;

}


int ProbePvTable(S_BOARD *board){

    int index = board->posKey % board->pvTable->num_entires;
    if(board->pvTable->pTable[index].poskey == board->posKey){
        return board->pvTable->pTable[index].move;
    }

    return NOMOVE;
    
}
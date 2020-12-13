#include "ark.h"


int GetPvLine(S_BOARD *board, int depth){


    ASSERT(depth < MAX_DEPTH);

    int move = ProbePvTable(board);
    int curr_ply = board->ply;
    int count = 0;

    while(move && count < depth){

        if(MoveExists(board, move)){
            board->pvArray[count++] = move;
            MakeMove(board, move);
        }else{
            break;
        }
        move = ProbePvTable(board);
    }

    while(board->ply > curr_ply){
        TakeMove(board);
    }

    return count;

}




const int pvSize = 0x100000 * 2; // 2MB of space

void ClearTable(S_PVTABLE *table){

    for(int i = 0; i < table->num_entires; i++){
        table->pTable[i].move = NOMOVE;
        table->pTable[i].poskey = 0ULL;
    }

}


void InitPvTable(S_PVTABLE *table){

    int size = pvSize / sizeof(S_PVENTRY);
    table->num_entires = size - 2;
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
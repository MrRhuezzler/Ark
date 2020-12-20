#include "ark.h"

// Resets the frst bit set to 1 and returns the index
int popBit(U64 *bb){
    for(int i = 0; i < 64; i++){
        if(*bb & (1ULL << i)){
            *bb &= ~(1ULL << i);
            return i;
        }
    }
}

// Counts the number of 1 in the bitmap
int countBit(U64 bb){
    int count = 0;
    for(int i = 0; i < 64; i++){
        if(bb & (1ULL << i)){
            count++;
        }
    }
    return count;
}

// Prints the bitboard to the console
void printBitBoard(U64 bb){
    printf("\n\n");

    U64 map = 1ULL;

    for(int rank = RANK_8; rank >= RANK_1; rank--){
        for(int file = FILE_A; file <= FILE_H; file++){
            int sq = FR2SQ(file, rank);
            int sq64 = SQ64(sq);
            if(bb & (map << sq64)){
                printf("1 ");
            }else{
                printf("0 ");
            }
        }
        printf("\n");
    }
    printf("\n\n");


}
#include "ark.h"

int SQ120toSQ64[BRD_SQ_NUM];
int SQ64toSQ120[64];

U64 SetMask[64];
U64 ClearMask[64];

U64 pieceKey[13][120];
U64 sideKey;
U64 castlePemKey[16];

char PieceChars[]  = ".PNBRQKpnbrqk";
char FileChars[]  = "ABCDEFGH";
char RankChars[] = "12345678";
char SideChars[]  = "WB-";

int PieceBig[13] = {FALSE, FALSE,  TRUE,  TRUE, TRUE, TRUE,   TRUE, FALSE,  TRUE,  TRUE, TRUE, TRUE, TRUE};
int PieceMaj[13] = {FALSE, FALSE, FALSE,FALSE, TRUE,  TRUE,   TRUE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE};
int PieceMin[13] = {FALSE, FALSE,  TRUE, TRUE, FALSE, FALSE,  FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE};
int PieceVal[13] = {0, 100, 325, 325, 550, 1000, 50000, 100, 352, 352, 550, 1000, 50000};
int PieceCol[13] = {BOTH, WHITE,  WHITE, WHITE, WHITE, WHITE, WHITE, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK};


int FilesBrd[BRD_SQ_NUM];
int RanksBrd[BRD_SQ_NUM];

int IsPawn[13] = {FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE};
int IsKnight[13] = {FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE};
int IsBishopQueen[13] = {FALSE, FALSE, FALSE, wB, FALSE, TRUE, FALSE, FALSE, FALSE, TRUE, FALSE, TRUE, FALSE};
int IsRookQueen[13] = {FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, FALSE};
int IsKing[13] = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE};

void InitRANKS_AND_FILES(){

    for(int i = 0; i < BRD_SQ_NUM; i++){
        FilesBrd[i] = OFF_BOARD;
        RanksBrd[i] = OFF_BOARD;
    }

    for(int rank = RANK_1; rank <= RANK_8; rank++){
        for(int file = FILE_A; file <= FILE_H; file++){
            int sq = FR2SQ(file, rank);
            FilesBrd[sq] = file;
            RanksBrd[sq] = rank;
        }
    }

}



void InitHASHKEYS(){

    for(int i = 0;i < 13; i++)
        for(int j = 0; j < 120; j++){
            pieceKey[i][j] = RAND_64;
        }
    
    sideKey = RAND_64;
    for(int i = 0; i < 16; i++){
        castlePemKey[i] = RAND_64;
    }

}

void InitBITMASKS(){
    for(int i = 0; i < 64; i++){
        SetMask[i] = (1ULL << i);
        ClearMask[i] = ~SetMask[i];
    }
}

void InitSQUARES(){

    for(int i = 0; i < BRD_SQ_NUM; i++){
        SQ120toSQ64[i] = NO_SQ;
    }

    for(int i = 0; i < 64; i++){
        SQ64toSQ120[i] = NO_SQ;
    }

    int sq64 = 0;

    // Mapping from 64 to 120 and 120 to 64
    for(int rank = RANK_1; rank <= RANK_8; rank++){
        for(int file = FILE_A; file <= FILE_H; file++){
            int sq = FR2SQ(file, rank);
            SQ120toSQ64[sq] = sq64;
            SQ64toSQ120[sq64] = sq;
            sq64++;
        }
    }

}

void Init(){
    InitSQUARES();
    InitBITMASKS();
    InitHASHKEYS();
    InitRANKS_AND_FILES();
}
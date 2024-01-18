#ifndef ARK_H
#define ARK_H

/* Library Headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <ctype.h>
#include <fcntl.h>

/* Local Header */
#include "constants.h"
#include "structures.h"

/* --------------- MACROS --------------- */

// #define DEBUG // Remove this during Release
// Assert Function for checking conditions (Only in Debug Mode)
#ifndef DEBUG
#define ASSERT(n)
#else
#define ASSERT(n)                         \
    if (!(n))                             \
    {                                     \
        printf("%s - Failed ", #n);       \
        printf("In File %s ", __FILE__);  \
        printf("At Line %d\n", __LINE__); \
        exit(1);                          \
    }
#endif

// Macro to get from FILE, RANK to SQUARE INDEX
#define FR2SQ(f, r) (A1 + (f)) + ((r)*10)

// Macro for popBit
#define POP(bb) popBit(bb)

// Macro for countBit
#define CNT(bb) countBit(bb)

// Macros for set and clear bit
#define CLRBIT(bb, b) (bb &= ClearMask[(b)])
#define SETBIT(bb, b) (bb |= SetMask[(b)])

// Macro for sq64 to sq120 Conversion
#define SQ64(sq120) (SQ120toSQ64[(sq120)])
#define SQ120(sq64) (SQ64toSQ120[(sq64)])

// Boolean Arrays
#define IsKn(p) IsKnight[p]
#define IsBQ(p) IsBishopQueen[p]
#define IsRQ(p) IsRookQueen[p]
#define IsKi(p) IsKing[p]

// Random Key Generator (ULONGLONG)
#define RAND_64 ((U64)rand() |       \
                 (U64)rand() << 15 | \
                 (U64)rand() << 30 | \
                 (U64)rand() << 45 | \
                 (U64)rand() << 60)

// Move preprocessors
#define FROSQ(move) (move & 0x7F)
#define TOSQ(move) ((move >> 7) & 0x7F)
#define CAPTURED(move) ((move >> 14) & 0xF)
#define PROMOTION(move) ((move >> 20) & 0xF)

// Flags
#define MFLAGEP 0x40000
#define MFLAGPS 0x80000
#define MFLAGCA 0x1000000

#define MFLAGCAP 0x7C000
#define MFLAGPRO 0xF00000

// Move Generators
#define MOVE(f, t, ca, pro, fl) ((f) | ((t) << 7) | ((ca) << 14) | ((pro) << 20) | ((fl)))
#define SQOFFBOARD(sq) (FilesBrd[(sq)] == OFF_BOARD)

// MakeMove
#define HASHPCE(pce, sq) (board->posKey ^= pieceKey[(pce)][(sq)])
#define HASHCA (board->posKey ^= castlePemKey[board->castlePerm])
#define HASHSIDE (board->posKey ^= (sideKey))
#define HASHEP (board->posKey ^= pieceKey[EMPTY][board->enPass])

// Io
#define NOMOVE 0

/* --------------- GLOBALS --------------- */

// Mapping from 120grid to 64 grid
extern int SQ120toSQ64[BRD_SQ_NUM];

// Mapping from 64grid to 120 grid
extern int SQ64toSQ120[64];

// Set and Clear Masks
extern U64 SetMask[64];
extern U64 ClearMask[64];

// Set of unique keys to generate posKey for a board
extern U64 pieceKey[13][120];
extern U64 sideKey;
extern U64 castlePemKey[16];

// Arrays used for printing purposes
extern char RankChars[];
extern char FileChars[];
extern char PieceChars[];
extern char SideChars[];
extern char NotationChars[];
extern wchar_t WCharPieces[];

// Boolean Array to find which type of piece is it
extern int PieceBig[13];
extern int PieceMaj[13];
extern int PieceMin[13];
extern int PieceVal[13];
extern int PieceCol[13];

// Given sq to find which file or rank it belongs
extern int FilesBrd[BRD_SQ_NUM];
extern int RanksBrd[BRD_SQ_NUM];

// Is  particular piece
extern int IsKnight[13];
extern int IsBishopQueen[13];
extern int IsRookQueen[13];
extern int IsKing[13];
extern int IsPawn[13];

/* --------------- FUNCTIONS --------------- */

// init.c
extern void Init();

// bitboard.c
extern void printBitBoard(U64 bb);
extern int popBit(U64 *bb);
extern int countBit(U64 bb);

// haskey.c
extern U64 generatePosKey(const S_BOARD *board);

// board.c
extern void resetBoard(S_BOARD *board);
extern int parseFen(const char *fen, S_BOARD *board);
extern void printBoard(const S_BOARD *board);
extern void UpdateListsMaterial(S_BOARD *board);
extern int CheckBoard(const S_BOARD *board);
void printBoardOnly(const S_BOARD *board);

// attack.c
extern int isSqAttacked(const int sq, const int side, const S_BOARD *board);
extern int isSqDoublePiece(const int gpiece, const int g_sq, const int sq, const S_BOARD *board);

// io.c
extern void SqAt(const int side, const S_BOARD *board);
extern char *PrSq(const int sq);
extern char *PrMove(const int move);
extern void PrintMoveList(const S_MOVELIST *list);
extern int ParseInputMove(char *input, S_BOARD *board);
extern char *HNPrMove(const int move, S_BOARD *board);

// validate.c
extern int SqOnBoard(const int sq);
extern int SideValid(const int side);
extern int FileRankValid(const int fr);
extern int PieceValidEmpty(const int pce);
extern int PieceValid(const int pce);

// movegen.c
extern void InitMvvLVA();
extern int MoveExists(S_BOARD *board, const int move);
extern void GenerateAllMoves(const S_BOARD *board, S_MOVELIST *list);
extern void GenerateAllCaptures(const S_BOARD *board, S_MOVELIST *list);

// makemove.c
extern int MakeMove(S_BOARD *board, const int move);
extern void TakeMove(S_BOARD *board);

// perft.c
extern void perftTest(int depth, S_BOARD *board);

// search.c
extern void SearchPosition(S_BOARD *board, S_SEARCHINFO *info);

// misc.c
extern U64 getMillis();
extern void ReadInput(S_SEARCHINFO *info);

// pvtable.c
extern void InitPvTable(S_PVTABLE *table);
extern void ClearTable(S_PVTABLE *table);
extern int ProbePvTable(S_BOARD *board);
extern void StorePvTable(S_BOARD *board, int move);
int GetPvLine(S_BOARD *board, int depth);

// evaluate.c
extern int EvalPosition(const S_BOARD *board);

// uci.c
extern void UCILoop();
#endif
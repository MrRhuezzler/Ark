#ifndef MAIN_H
#define MAIN_H

#include "constants.h"

typedef struct
{
    int move;
    int score;

} S_MOVE;

/* GAME MOVE 

20 < FROM, TO < 99 => Approx 7 bits are needed

0000 0000 0000 0000 0000 0111 1111 => FROM (0x7F)
0000 0000 0000 0011 1111 1000 0000 => TO (0x7F << 7)
0000 0000 0011 1100 0000 0000 0000 => Captured Piece (F << 14)
0000 0000 0100 0000 0000 0000 0000 => EnPass (0x40000)
0000 0000 1000 0000 0000 0000 0000 => PawnStart (0x80000)
0000 1111 0000 0000 0000 0000 0000 => Promoted Piece (F << 20)
0001 0000 0000 0000 0000 0000 0000 => IsCastle (0x1000000)

GAME MOVE */

typedef struct
{

    S_MOVE moves[MAX_POSTION_MOVES];
    int count;

} S_MOVELIST;


// UNDO Representation
typedef struct 
{

    int move;
    int castlePerm;
    int enPass;
    int fiftyMoves;

    U64 posKey;


} S_UNDO;



// Board Representation
typedef struct 
{

    // Represent the pieces on the board
    int pieces[BRD_SQ_NUM];

    // 00000000 01010000 ... 00000000 => b2, d2 has a pawn
    // Easier to decide which FILES or RANKS are OPEN, DOUBLE PAWNS
    U64 pawns[3];

    int kingSq[2];

    // WHITE or BLACK is currently playing
    int side;

    // Is there a enPassent square
    int enPass;
    // If no captures takes place for 50 moves then its a draw
    int fiftyMoves;

    // Total number of half games => ply
    int ply;

    // Used to index history
    int hisply;

    // Possible Castling moves in the boards
    int castlePerm;

    // Total Amount of pieces

    // Amount of eact piece in the board 
    int pceNum[13];
    // Amount of big pieces in the board (all execpt pawns)
    int bigPce[2];
    // Amount of major pieces (Queen, Rook)
    int majorPce[2];
    // Amount of minor pieces (Bishop, Knight)
    int minorPce[2];
    // Material Value
    int material[2];
    
    // Game History Array
    // To Also check posKey matches then position repeats
    S_UNDO history[MAX_GAME_MOVES];

    // Unique key for the current board
    U64 posKey;

    // Piece List
    int pList[13][10];

    // pList[wN][0] = E1;
    // pList[wN][1] = D4;


} S_BOARD;



#endif
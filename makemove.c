#include "ark.h"

const int CastlePerm[120] = {
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 13, 15, 15, 15, 12, 15, 15, 14, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15,  7, 15, 15, 15,  3, 15, 15, 11, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
};


static void ClearPiece(const int sq, S_BOARD *board){
    ASSERT(SqOnBoard(sq));

    int piece = board->pieces[sq];

    ASSERT(PieceValid(piece));
    int col = PieceCol[piece];
    int t_pceNum = -1;

    // Hashing the piece out from Poskey 
    HASHPCE(piece, sq);

    board->pieces[sq] = EMPTY;
    board->material[col] -= PieceVal[piece];

    if(PieceBig[piece]){
        // Other than Pawns
        board->bigPce[col]--;
        if(PieceMaj[piece]){
            board->majorPce[col]--;
        }else{
            board->minorPce[col]--;
        }


    }else{
        // Pawn
        CLRBIT(board->pawns[col], SQ64(sq));
        CLRBIT(board->pawns[BOTH], SQ64(sq));

    }

    // Removing from pList
    for(int i = 0; i < board->pceNum[piece]; i++){
        if(board->pList[piece][i] == sq){
            t_pceNum = i;
            break;
        }
    }

    ASSERT(t_pceNum != -1);

    board->pceNum[piece]--;

    board->pList[piece][t_pceNum] = board->pList[piece][board->pceNum[piece]];

}

static void AddPiece(const int sq, S_BOARD *board, const int piece){

    ASSERT(PieceValid(piece));
    ASSERT(SqOnBoard(sq));

    int col = PieceCol[sq];

    HASHPCE(piece, sq);

    board->pieces[sq] = piece;

    // Incrementing Things

    if(PieceBig[piece]){
        board->bigPce[col]++;
        if(PieceMaj[piece]){
            board->majorPce[col]++;
        }else{
            board->minorPce[col]++;
        }
    }else{
        SETBIT(board->pawns[col], SQ64(sq));
        SETBIT(board->pawns[BOTH], SQ64(sq));
    }

    // Adding to pList

    board->material[col] += PieceVal[piece];
    board->pList[piece][board->pceNum[piece]++] = sq;

}


static void MovePiece(const int from, const int to, S_BOARD *board){

    ASSERT(SqOnBoard(from));
    ASSERT(SqOnBoard(to));


    int piece = board->pieces[from];
    int col = PieceCol[piece];

    #ifdef DEBUG
    int t_pceNum = FALSE;
    #endif

    // Hashing out piece from from_sq
    HASHPCE(piece, from);
    board->pieces[from] = EMPTY;

    // Hashing in piece from to_sq
    HASHPCE(piece, to);
    board->pieces[to] = piece;
    
    // Pawns to clear and set bits
    if(!PieceBig[piece]){
        CLRBIT(board->pawns[col], SQ64(from));
        CLRBIT(board->pawns[BOTH], SQ64(from));
        SETBIT(board->pawns[col], SQ64(to));
        SETBIT(board->pawns[BOTH], SQ64(to));        
    }

    // pList changes
    for(int i = 0; i < board->pceNum[piece]; i++){
        if(board->pList[piece][i] == from){
            board->pList[piece][i] = to;
            
            #ifdef DEBUG
            t_pceNum = TRUE;
            #endif

            break;
        }
    }

    ASSERT(t_pceNum);

}

// Return a int if TRUE then the current player has left them in check
// which is considered to be illegal
int MakeMove(S_BOARD *board, int move){


    ASSERT(CheckBoard(board));

    int from = FROSQ(move);
    int to = TOSQ(move);
    int side = board->side;

    ASSERT(SqOnBoard(from));
    ASSERT(SqOnBoard(to));
    ASSERT(SideValid(side));
    ASSERT(PieceValid(board->pieces[from]));

    // hisply => total half moves played
    // ply => total moves into the search ( resets to zero )

    board->history[board->hisply].posKey = board->posKey;

    // EnPass Move
    if(move & MFLAGEP){
        if(side == WHITE){
            ClearPiece(to - 10, board);
        }else{
            ClearPiece(to + 10, board);
        }

    // Castle Move
    }else if(move & MFLAGCA){
        switch (to)
        {
        case C1:
            MovePiece(A1, D1, board);
            break;
        case C8:
            MovePiece(A8, D8, board);
            break;
        case G1:
            MovePiece(H1, F1, board);
            break;
        case G8:
            MovePiece(H8, F8, board);
            break;
        default:
            ASSERT(FALSE);
            break;
        }
    }

    if(board->enPass != NO_SQ) HASHEP;
    HASHCA;

    board->history[board->hisply].move = move;
    board->history[board->hisply].fiftyMoves = board->fiftyMoves;
    board->history[board->hisply].enPass = board->enPass;
    board->history[board->hisply].castlePerm = board->castlePerm;


    board->castlePerm &= CastlePerm[from];
    board->castlePerm &= CastlePerm[to];
    board->enPass;

    HASHCA;

    int cap = CAPTURED(move);
    board->fiftyMoves++;

    if(cap != EMPTY){
        ASSERT(PieceValid(cap));
        ClearPiece(to, board);
        board->fiftyMoves = 0;
    }

    board->hisply++;
    board->ply++;

    if(IsPawn[board->pieces[from]]){
        board->fiftyMoves = 0;
        if(move & MFLAGEP){
            if(side == WHITE){
                board->enPass = from + 10;
                ASSERT(RanksBrd[board->enPass] == RANK_3);
            }else{
                board->enPass = from - 10;
                ASSERT(RanksBrd[board->enPass] == RANK_6);
            }

            HASHEP;
        }
    }

    MovePiece(from, to, board);

    int prPiece = PROMOTION(move);

    if(prPiece != EMPTY){
        ASSERT(PieceValid(prPiece) && !IsPawn[prPiece]);
        ClearPiece(to, board);
        AddPiece(to, board, prPiece);
    }

    if(IsKing[board->pieces[to]]){
        board->kingSq[board->side] = to;
    }

    board->side ^= 1;
    HASHSIDE;

    ASSERT(CheckBoard(board));
    
    if(isSqAttacked(board->kingSq[board->side], board->side, board)){
        TakeMove(board);
        return FALSE;
    }

    return TRUE;

}

void TakeMove(S_BOARD *board){

    ASSERT(CheckBoard(board));
    
    board->hisply--;
    board->ply--;

    int move = board->history[board->hisply].move;
    int from = FROSQ(move);
    int to = TOSQ(move);

    ASSERT(SqOnBoard(from));
    ASSERT(SqOnBoard(to));

    if(board->enPass != NO_SQ) HASHEP;
    HASHCA;


    board->castlePerm = board->history[board->hisply].castlePerm;
    board->fiftyMoves = board->history[board->hisply].fiftyMoves;
    board->enPass = board->history[board->hisply].castlePerm;

    if(board->enPass != NO_SQ) HASHEP;
    HASHCA;

    board->side ^= 1;
    HASHSIDE;

    if(move & MFLAGEP){
        if(board->side == WHITE){
            AddPiece(to - 10, board, bP);
        }else{
            AddPiece(to + 10, board, wP);
        }
    }else if(move & MFLAGCA){
        switch (to)
        {
        case C1:
            MovePiece(D1, A1, board);
            break;
        case C8:
            MovePiece(D8, A8, board);
            break;
        case G1:
            MovePiece(F1, H1, board);
            break;
        case G8:
            MovePiece(F8, H8, board);
            break;
        default:
            ASSERT(FALSE);
            break;
        }
    }

    MovePiece(to, from, board);

    if(IsKing[board->pieces[from]]){
        board->kingSq[board->side] = from;
    }

    int cap = CAPTURED(move);
    if(cap != EMPTY){
        ASSERT(PieceValid(cap));
        AddPiece(to, from, cap);
    }

    int prPiece = PROMOTION(move);

    if(prPiece != EMPTY){
        ASSERT(PieceValid(prPiece) && !IsPawn[prPiece]);
        ClearPiece(from, board);
        AddPiece(from, board, (PieceCol[prPiece] == WHITE) ? wP : bP );
    }

    ASSERT(CheckBoard(board));

}
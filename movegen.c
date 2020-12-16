#include "ark.h"

/*

Generate Moves(board, list)
    Iterate over the pieces
        -> Slider loop in all posible directions and add move
            -> Add Move list->moves[list->count]; list->count++;

*/

// Move Order
// PV TABLE
// Most Valuable Victim Least Valuable Attacker

/*
* MvvLVA
*  [Vic][Att] = score such that high for P x Q and low for Q x P
* P x Q
* N x Q
* B x Q
* R x Q
* Q x Q
*
* P x R
* N x R
* B x R
* R x R
* Q x R
* ...
* B
* N
* P
* ...
*
*/




// Killer Moves
// Search History

// Used to set the score in the move generator

const int VictimScore[13] = { 0, 100, 200, 300, 400, 500, 600, 100, 200, 300, 400, 500, 600 };

static int MvvLVA[13][13];

void InitMvvLVA() {

    int attacker, victim;

    for (victim = wP; victim <= bK; victim++) {
        for (attacker = wP; attacker <= bK; attacker++) {
            MvvLVA[victim][attacker] = VictimScore[victim] + 6 - (VictimScore[attacker] / 100);
        }
    }

}


const int sliderPieces[] = {wB, wR, wQ, 0, bB, bR, bQ, 0};
const int jumpingPieces[] = {wN, wK, 0, bN, bK, 0};

const int sliderPiecesIndex[] = {0, 4};
const int jumpingPiecesIndex[] = {0, 3};

const int piecesDir[13][8] = {
    {0},
    {0},
    {+8, +12, +19, +21, -8, -12, -19, -21},
    {-9, -11, 11, 9},
    {-1, -10, 1, 10},
    {-1, -10, 1, 10, -9, -11, 11, 9},
    {-9, -11, 11, 9, -1, -10, 1, 10},
    {0},
    {+8, +12, +19, +21, -8, -12, -19, -21},
    {-9, -11, 11, 9},
    {-1, -10, 1, 10},
    {-1, -10, 1, 10, -9, -11, 11, 9},
    {-9, -11, 11, 9, -1, -10, 1, 10},
};

const int numDir[13] = {0, 0, 8, 4, 4, 8, 8, 0, 8, 4, 4, 8, 8};

// To Check whether a move exists
int MoveExists(S_BOARD *board, const int move){

    S_MOVELIST list[1];
    GenerateAllMoves(board, list);

    for(int i = 0; i < list->count; i++){

        if(!MakeMove(board, list->moves[i].move)){
            continue;
        }

        TakeMove(board);

        if(list->moves[i].move == move){
            return TRUE;
        }

    }

    return FALSE;

}


// Adding a normal move
static void addQuietMove(const S_BOARD *board, int move, S_MOVELIST *list){
    list->moves[list->count].move = move;

    if (board->searchKillers[0][board->ply] == move) {
        list->moves[list->count].score = 900000;
    }
    else if (board->searchKillers[1][board->ply] == move) {
        list->moves[list->count].score = 800000;
    }
    else {
        list->moves[list->count].score = board->searchHistory[board->pieces[FROSQ(move)]][TOSQ(move)];
    }
    // list->moves[list->count].score = 0;
    list->count++;
}

// Adding a Capture move
static void addCaptureMove(const S_BOARD *board, int move, S_MOVELIST *list){
    list->moves[list->count].move = move;
    list->moves[list->count].score = MvvLVA[CAPTURED(move)][board->pieces[FROSQ(move)]] + 1000000;
    list->count++;
}

// Adding a EnPass move
static void addEnPassMove(const S_BOARD *board, int move, S_MOVELIST *list){
    list->moves[list->count].move = move;
    list->moves[list->count].score = 105 + 1000000;
    list->count++;
}

// Adds the white pawn's possible captures moves 
static void addWhitePawnCaptureMoves(const S_BOARD *board, const int from, const int to, const int cap, S_MOVELIST *list){

    ASSERT(PieceValidEmpty(cap));
    ASSERT(SqOnBoard(from));
    ASSERT(SqOnBoard(to));

    if(RanksBrd[from] == RANK_7){
        addCaptureMove(board, MOVE(from, to, cap, wQ, 0), list);
        addCaptureMove(board, MOVE(from, to, cap, wR, 0), list);
        addCaptureMove(board, MOVE(from, to, cap, wN, 0), list);
        addCaptureMove(board, MOVE(from, to, cap, wB, 0), list);
    }else{
        addCaptureMove(board, MOVE(from, to, cap, EMPTY, 0), list);
    }
}

// Adds the white pawn's possible quiet moves 
static void addWhitePawnQuietMoves(const S_BOARD *board, const int from, const int to, S_MOVELIST *list){

    ASSERT(SqOnBoard(from));
    ASSERT(SqOnBoard(to));

    if(RanksBrd[from] == RANK_7){
        addQuietMove(board, MOVE(from, to, EMPTY, wQ, 0), list);
        addQuietMove(board, MOVE(from, to, EMPTY, wR, 0), list);
        addQuietMove(board, MOVE(from, to, EMPTY, wN, 0), list);
        addQuietMove(board, MOVE(from, to, EMPTY, wB, 0), list);
    }else{
        addQuietMove(board, MOVE(from, to, EMPTY, EMPTY, 0), list);
    }
}



// Adds the black pawn's possible captures moves 
static void addBlackPawnCaptureMoves(const S_BOARD *board, const int from, const int to, const int cap, S_MOVELIST *list){

    ASSERT(PieceValidEmpty(cap));
    ASSERT(SqOnBoard(from));
    ASSERT(SqOnBoard(to));

    if(RanksBrd[from] == RANK_2){
        addCaptureMove(board, MOVE(from, to, cap, bQ, 0), list);
        addCaptureMove(board, MOVE(from, to, cap, bR, 0), list);
        addCaptureMove(board, MOVE(from, to, cap, bN, 0), list);
        addCaptureMove(board, MOVE(from, to, cap, bB, 0), list);
    }else{
        addCaptureMove(board, MOVE(from, to, cap, EMPTY, 0), list);
    }
}

// Adds the black pawn's possible quiet moves 
static void addBlackPawnQuietMoves(const S_BOARD *board, const int from, const int to, S_MOVELIST *list){

    ASSERT(SqOnBoard(from));
    ASSERT(SqOnBoard(to));

    if(RanksBrd[from] == RANK_2){
        addQuietMove(board, MOVE(from, to, EMPTY, bQ, 0), list);
        addQuietMove(board, MOVE(from, to, EMPTY, bR, 0), list);
        addQuietMove(board, MOVE(from, to, EMPTY, bN, 0), list);
        addQuietMove(board, MOVE(from, to, EMPTY, bB, 0), list);
    }else{
        addQuietMove(board, MOVE(from, to, EMPTY, EMPTY, 0), list);
    }
}





void GenerateAllMoves(const S_BOARD *board, S_MOVELIST *list){


    ASSERT(CheckBoard(board));

    list->count = 0;
    int side = board->side;

    if(side == WHITE){
        // White to Play

        // Pawn
        for(int pceNum = 0; pceNum < board->pceNum[wP]; pceNum++){

            int sq = board->pList[wP][pceNum];
            ASSERT(SqOnBoard(sq));
            // One Step forward
            if(board->pieces[sq + 10] == EMPTY){
                addWhitePawnQuietMoves(board, sq, sq + 10, list);
                // Two Step forward when RANK == 2
                if(RanksBrd[sq] == RANK_2 && board->pieces[sq + 20] == EMPTY){
                    addQuietMove(board, MOVE(sq, sq + 20, EMPTY, EMPTY, MFLAGPS), list);
                }
            }

            // Capture
            if(!SQOFFBOARD(sq + 9) && PieceCol[board->pieces[sq + 9]] == BLACK){
                addWhitePawnCaptureMoves(board, sq, sq + 9, board->pieces[sq + 9], list);
            }

            if(!SQOFFBOARD(sq + 11) && PieceCol[board->pieces[sq + 11]] == BLACK){
                addWhitePawnCaptureMoves(board, sq, sq + 11, board->pieces[sq + 11], list);
            }

            if(board->enPass != NO_SQ){
                if((sq + 9) == board->enPass){
                    addEnPassMove(board, MOVE(sq, sq + 9, EMPTY, EMPTY, MFLAGEP), list);
                }

                if((sq + 11) == board->enPass){
                    addEnPassMove(board, MOVE(sq, sq + 11, EMPTY, EMPTY, MFLAGEP), list);
                }
            }
        }

        if(board->castlePerm & WKCA){
            if(board->pieces[F1] == EMPTY && board->pieces[G1] == EMPTY){
                if(!isSqAttacked(E1, BLACK, board) && !isSqAttacked(F1, BLACK, board)){
                    addQuietMove(board, MOVE(E1, G1, EMPTY, EMPTY, MFLAGCA), list);
                }
            }
        }

        if(board->castlePerm & WQCA){
            if(board->pieces[D1] == EMPTY && board->pieces[C1] == EMPTY  && board->pieces[B1] == EMPTY){
                if(!isSqAttacked(E1, BLACK, board) && !isSqAttacked(D1, BLACK, board)){
                    addQuietMove(board, MOVE(E1, C1, EMPTY, EMPTY, MFLAGCA), list);
                }
            }
        }

    }else{

        // Black to play

        // Pawn
        for(int pceNum = 0; pceNum < board->pceNum[bP]; pceNum++){

            int sq = board->pList[bP][pceNum];
            ASSERT(SqOnBoard(sq));
            // One Step forward
            if(board->pieces[sq - 10] == EMPTY){
                addBlackPawnQuietMoves(board, sq, sq - 10, list);
                // Two Step forward when RANK == 7
                if(RanksBrd[sq] == RANK_7 && board->pieces[sq - 20] == EMPTY){
                    addQuietMove(board, MOVE(sq, sq - 20, EMPTY, EMPTY, MFLAGPS), list);
                }
            }

            // Capture
            if(!SQOFFBOARD(sq - 9) && PieceCol[board->pieces[sq - 9]] == WHITE){
                addBlackPawnCaptureMoves(board, sq, sq - 9, board->pieces[sq - 9], list);
            }

            if(!SQOFFBOARD(sq - 11) && PieceCol[board->pieces[sq - 11]] == WHITE){
                addBlackPawnCaptureMoves(board, sq, sq - 11, board->pieces[sq - 11], list);
            }

            if(board->enPass != NO_SQ){
                if((sq - 9) == board->enPass){
                    addEnPassMove(board, MOVE(sq, sq - 9, EMPTY, EMPTY, MFLAGEP), list);
                }

                if((sq - 11) == board->enPass){
                    addEnPassMove(board, MOVE(sq, sq - 11, EMPTY, EMPTY, MFLAGEP), list);
                }
            }
        }


        if(board->castlePerm & BKCA){
            if(board->pieces[F8] == EMPTY && board->pieces[G8] == EMPTY){
                if(!isSqAttacked(E8, WHITE, board) && !isSqAttacked(F8, WHITE, board)){
                    addQuietMove(board, MOVE(E8, G8, EMPTY, EMPTY, MFLAGCA), list);
                }
            }
        }

        if(board->castlePerm & BQCA){
            if(board->pieces[D8] == EMPTY && board->pieces[C8] == EMPTY  && board->pieces[B8] == EMPTY){
                if(!isSqAttacked(E8, WHITE, board) && !isSqAttacked(D8, WHITE, board)){
                    addQuietMove(board, MOVE(E8, C8, EMPTY, EMPTY, MFLAGCA), list);
                }
            }
        }


    }



    // Slider Pieces

    int i = sliderPiecesIndex[board->side];
    int piece = sliderPieces[i++];

    while(piece){

        ASSERT(PieceValid(piece));

        for(int j = 0; j < board->pceNum[piece]; j++){

            int sq = board->pList[piece][j];

            ASSERT(SqOnBoard(sq));

            for(int k = 0; k < numDir[piece]; k++){
                int t_sq = sq + piecesDir[piece][k];

                while(!SQOFFBOARD(t_sq)){
                    if(board->pieces[t_sq] != EMPTY){
                        if(PieceCol[board->pieces[t_sq]] == (board->side ^ 1)){
                            addCaptureMove(board, MOVE(sq, t_sq, board->pieces[t_sq], EMPTY, 0), list);
                        }
                        break;
                    }
                    addQuietMove(board, MOVE(sq, t_sq, EMPTY, EMPTY, 0), list);
                    t_sq += piecesDir[piece][k];
                }
            }
        }
        piece = sliderPieces[i++];
    }



    // Jumping Pieces

    i = jumpingPiecesIndex[board->side];
    piece = jumpingPieces[i++];

    while(piece){

        ASSERT(PieceValid(piece));

        for(int j = 0; j < board->pceNum[piece]; j++){

            int sq = board->pList[piece][j];

            ASSERT(SqOnBoard(sq));

            for(int k = 0; k < numDir[piece]; k++){

                int t_sq = sq + piecesDir[piece][k];

                if (SQOFFBOARD(t_sq)){
                    continue;
                }

                if(board->pieces[t_sq] != EMPTY){
                    if(PieceCol[board->pieces[t_sq]] == (board->side ^ 1)){
                        addCaptureMove(board, MOVE(sq, t_sq, board->pieces[t_sq], EMPTY, 0), list);
                    }
                    continue;
                }
                addQuietMove(board, MOVE(sq, t_sq, EMPTY, EMPTY, 0), list);

            }

        }
        piece = jumpingPieces[i++];
    }

}




void GenerateAllCaptures(const S_BOARD* board, S_MOVELIST* list) {


    ASSERT(CheckBoard(board));

    list->count = 0;
    int side = board->side;

    if (side == WHITE) {
        // White to Play

        // Pawn
        for (int pceNum = 0; pceNum < board->pceNum[wP]; pceNum++) {

            int sq = board->pList[wP][pceNum];
            ASSERT(SqOnBoard(sq));

            // Capture
            if (!SQOFFBOARD(sq + 9) && PieceCol[board->pieces[sq + 9]] == BLACK) {
                addWhitePawnCaptureMoves(board, sq, sq + 9, board->pieces[sq + 9], list);
            }

            if (!SQOFFBOARD(sq + 11) && PieceCol[board->pieces[sq + 11]] == BLACK) {
                addWhitePawnCaptureMoves(board, sq, sq + 11, board->pieces[sq + 11], list);
            }

            if (board->enPass != NO_SQ) {
                if ((sq + 9) == board->enPass) {
                    addEnPassMove(board, MOVE(sq, sq + 9, EMPTY, EMPTY, MFLAGEP), list);
                }

                if ((sq + 11) == board->enPass) {
                    addEnPassMove(board, MOVE(sq, sq + 11, EMPTY, EMPTY, MFLAGEP), list);
                }
            }
        }
    }
    else {

        // Black to play

        // Pawn
        for (int pceNum = 0; pceNum < board->pceNum[bP]; pceNum++) {

            int sq = board->pList[bP][pceNum];
            ASSERT(SqOnBoard(sq));

            // Capture
            if (!SQOFFBOARD(sq - 9) && PieceCol[board->pieces[sq - 9]] == WHITE) {
                addBlackPawnCaptureMoves(board, sq, sq - 9, board->pieces[sq - 9], list);
            }

            if (!SQOFFBOARD(sq - 11) && PieceCol[board->pieces[sq - 11]] == WHITE) {
                addBlackPawnCaptureMoves(board, sq, sq - 11, board->pieces[sq - 11], list);
            }

            if (board->enPass != NO_SQ) {
                if ((sq - 9) == board->enPass) {
                    addEnPassMove(board, MOVE(sq, sq - 9, EMPTY, EMPTY, MFLAGEP), list);
                }

                if ((sq - 11) == board->enPass) {
                    addEnPassMove(board, MOVE(sq, sq - 11, EMPTY, EMPTY, MFLAGEP), list);
                }
            }
        }
    }



    // Slider Pieces

    int i = sliderPiecesIndex[board->side];
    int piece = sliderPieces[i++];

    while (piece) {

        ASSERT(PieceValid(piece));

        for (int j = 0; j < board->pceNum[piece]; j++) {

            int sq = board->pList[piece][j];

            ASSERT(SqOnBoard(sq));

            for (int k = 0; k < numDir[piece]; k++) {
                int t_sq = sq + piecesDir[piece][k];

                while (!SQOFFBOARD(t_sq)) {
                    if (board->pieces[t_sq] != EMPTY) {
                        if (PieceCol[board->pieces[t_sq]] == (board->side ^ 1)) {
                            addCaptureMove(board, MOVE(sq, t_sq, board->pieces[t_sq], EMPTY, 0), list);
                        }
                        break;
                    }
                    t_sq += piecesDir[piece][k];
                }
            }
        }
        piece = sliderPieces[i++];
    }



    // Jumping Pieces

    i = jumpingPiecesIndex[board->side];
    piece = jumpingPieces[i++];

    while (piece) {

        ASSERT(PieceValid(piece));

        for (int j = 0; j < board->pceNum[piece]; j++) {

            int sq = board->pList[piece][j];

            ASSERT(SqOnBoard(sq));

            for (int k = 0; k < numDir[piece]; k++) {

                int t_sq = sq + piecesDir[piece][k];

                if (SQOFFBOARD(t_sq)) {
                    continue;
                }

                if (board->pieces[t_sq] != EMPTY) {
                    if (PieceCol[board->pieces[t_sq]] == (board->side ^ 1)) {
                        addCaptureMove(board, MOVE(sq, t_sq, board->pieces[t_sq], EMPTY, 0), list);
                    }
                    continue;
                }
            }

        }
        piece = jumpingPieces[i++];
    }

}

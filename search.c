#include "ark.h"

#define INFINITE 30000
#define MATE 29000

// Check if time up, or interrupt from GUI
static void CheckUp(S_SEARCHINFO *info){

    if (info->timeset == TRUE && getMills() > info->stoptime) {
        info->stopped = TRUE;
    }

}

static void PickBestMove(int moveNum, S_MOVELIST* list) {
    S_MOVE temp;
    int bestScore = 0;
    int bestNum = 0;
        
    for (int i = moveNum; i < list->count; i++) {
        if (list->moves[i].score > bestScore) {
            bestScore = list->moves[i].score;
            bestNum = i;
        }
    }

    temp = list->moves[bestNum];
    list->moves[bestNum] = list->moves[moveNum];
    list->moves[moveNum] = temp;

}

static int IsRepetition(const S_BOARD *board){
    
    
    for(int his = board->hisply - board->fiftyMoves; his < board->hisply - 1; his++){

        ASSERT(his >= 0 && his < MAX_GAME_MOVES);

        if(board->history[his].posKey == board->posKey){
            return TRUE;
        }
    }

    return FALSE;
}

// Clear things for a new search
static void ClearForSearch(S_BOARD *board, S_SEARCHINFO *info){

    for(int i = 0; i < 13 ; i++){
        for(int j = 0; j < BRD_SQ_NUM; j++){
            board->searchHistory[i][j] = 0;
        }
    }

    for(int i = 0; i < 2 ; i++){
        for(int j = 0; j < MAX_DEPTH; j++){
            board->searchKillers[i][j] = 0;
        }
    }

    ClearTable(board->pvTable);
    board->ply = 0;

    info->starttime = getMillis();
    info->stoptime = 0;
    info->nodes = 0;
    info->fhf = 0;
    info->fh = 0;

}

// Search for Quiet Moves
static int Quiescence(int alpha, int beta, S_BOARD *board,S_SEARCHINFO *info){


    ASSERT(CheckBoard(board));

    if (info->nodes % 2047 == 0) {
        CheckUp(info);
    }

    info->nodes++;

    if (IsRepetition(board) || board->fiftyMoves >= 100) {
        return 0;
    }

    // Thats the maximum u can go
    if (board->ply > MAX_DEPTH - 1) {
        return EvalPosition(board);
    }

    int score = EvalPosition(board);

    if (score >= beta) {
        return beta;
    }

    if (score > alpha) {
        alpha = score;
    }

    S_MOVELIST list[1];
    GenerateAllCaptures(board, list);

    int moveNum = 0;
    int legal = 0;
    int oldAlpha = alpha;
    int bestMove = NOMOVE;
    score = -INFINITE;

    for (moveNum = 0; moveNum < list->count; moveNum++) {

        PickBestMove(moveNum, list);

        if (!MakeMove(board, list->moves[moveNum].move)) {
            continue;
        }

        legal++;
        score = -Quiescence(-beta, -alpha, board, info);
        TakeMove(board);

        if (info->stopped == TRUE) {
            return 0;
        }

        if (score > alpha) {
            if (score >= beta) {
                // Beta cutoff
                if (legal == 1) {
                    info->fhf++;
                }
                info->fh++;
                return beta;
            }
            alpha = score;
            bestMove = list->moves[moveNum].move;
        }

    }

    if (alpha != oldAlpha) {
        StorePvTable(board, bestMove);
    }

    return alpha; 
}

static int AlphaBeta(int alpha, int beta, int depth, S_BOARD *board,S_SEARCHINFO *info, int DoNull){


    ASSERT(CheckBoard(board));

    if(depth == 0){
        return Quiescence(alpha, beta, board, info);
    }

    if (info->nodes % 2047 == 0) {
        CheckUp(info);
    }

    info->nodes++;

    if(IsRepetition(board) || board->fiftyMoves >= 100){
        return 0;
    }

    // Thats the maximum u can go
    if(board->ply > MAX_DEPTH - 1){
        return EvalPosition(board);
    }

    S_MOVELIST list[1];
    GenerateAllMoves(board, list);

    int moveNum = 0;
    int legal = 0;
    int oldAlpha = alpha;
    int bestMove = NOMOVE;
    int score = -INFINITE;
    int pvMove = ProbePvTable(board);

    if (pvMove != NOMOVE) {
        for (moveNum = 0; moveNum < list->count; moveNum++) {
            if (list->moves[moveNum].move == pvMove) {
                list->moves[moveNum].score = 2000000;
                break;
            }
        }
    }

    for(moveNum = 0; moveNum < list->count; moveNum++){

        PickBestMove(moveNum, list);

        if(!MakeMove(board, list->moves[moveNum].move)){
            continue;
        }

        legal++;
        score = -AlphaBeta(-beta, -alpha, depth - 1, board, info, TRUE);
        TakeMove(board);

        if (info->stopped == TRUE) {
            return 0;
        }

        if(score > alpha){
            if(score >= beta){
                // Beta cutoff
                if(legal == 1){
                    info->fhf++;
                }

                if (!(list->moves[moveNum].move & MFLAGCAP)) {
                    board->searchKillers[1][board->ply] = board->searchKillers[0][board->ply];
                    board->searchKillers[0][board->ply] = list->moves[moveNum].move;
                }

                info->fh++;
                return beta;
            }
            alpha = score;
            bestMove = list->moves[moveNum].move;
            if (!(list->moves[moveNum].move & MFLAGCAP)) {
                board->searchHistory[board->pieces[FROSQ(bestMove)]][TOSQ(bestMove)] += depth;
            }
        }

    }

    if(legal == 0){
        if(isSqAttacked(board->kingSq[board->side], board->side ^ 1, board)){
            // To Find the shortest path to mate
            return -MATE + board->ply;
        }else{
            return 0;
        }
    }

    if(alpha != oldAlpha){
        StorePvTable(board, bestMove);
    }

    return alpha;

}

void SearchPosition(S_BOARD *board, S_SEARCHINFO *info){
    // Iterative Deepening, search init

    // for depth equals 1 to MAX_DEPTH
        // search with alpha beta
        // next depth
        // Until time runs out

    // U can search for a particular depth, more efficient by Move Ordering and PV Table


    int bestMove = NOMOVE; // From PV TABLE
    int bestScore = -INFINITE; // From the current iteration
    int currentDepth = 0;
    int pvNum = 0;
    int pvMoves = 0;

    ClearForSearch(board, info);

    for(currentDepth = 1; currentDepth <= info->depth; currentDepth++){
        
        bestScore = AlphaBeta(-INFINITE, INFINITE, currentDepth, board, info, TRUE);

        // out of time?
        if (info->stopped == TRUE) {
            break;
        }

        pvMoves = GetPvLine(board, currentDepth);
        bestMove = board->pvArray[0];

        printf("Depth:%d score:%d move:%s nodes:%ld ",
            currentDepth, bestScore, PrMove(bestMove), info->nodes);

        pvMoves = GetPvLine(board, currentDepth);
        printf("pv");
        for (pvNum = 0; pvNum < pvMoves; ++pvNum) {
            printf(" %s", PrMove(board->pvArray[pvNum]));
        }
        printf("\n");
        printf("Ordering:%.2f\n", (info->fhf / info->fh));

    }

}
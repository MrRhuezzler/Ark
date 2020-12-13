#include "ark.h"

int getPiecefromChar(char ch){
    int piece;

    switch (ch)
    {
        case 'P': piece = wP; break; 
        case 'N': piece = wN; break;
        case 'B': piece = wB; break;
        case 'R': piece = wR; break;
        case 'Q': piece = wQ; break;
        case 'K': piece = wK; break;
        case 'p': piece = bP; break;
        case 'n': piece = bN; break;
        case 'b': piece = bB; break;
        case 'r': piece = bR; break;
        case 'q': piece = bQ; break;
        case 'k': piece = bK; break;

        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8': piece = EMPTY; break;

        default:
            piece = -1;
            break;

    }

    return piece;
}

char getCharfromPiece(int piece){

    char ch;

    switch (piece)
    {
    case wP:
    case bP: ch = 'P'; break;
    case wN:
    case bN: ch = 'N'; break;
    case wB:
    case bB: ch = 'B'; break;
    case wR:
    case bR: ch = 'R'; break;
    case wQ:
    case bQ: ch = 'Q'; break;
    case wK:
    case bK: ch = 'K'; break;
    
    default:
        ch = '-';
        break;
    }

    return ch;

}


int CheckBoard(const S_BOARD *board){

    int t_pceNum[13] = {0};
    int t_bigPce[2] = {0};
    int t_majorPce[2] = {0};
    int t_minorPce[2] = {0};
    int t_material[2] = {0};

    U64 t_pawns[3] = {0ULL, 0ULL, 0ULL};

    t_pawns[WHITE] = board->pawns[WHITE];
    t_pawns[BLACK] = board->pawns[BLACK];
    t_pawns[BOTH] = board->pawns[BOTH];

    // Check the position of the pieces
    for(int piece = wP; piece <= bK; piece++){
        for(int count = 0; count < board->pceNum[piece]; count++){
            int sq = board->pList[piece][count];
            ASSERT(piece == board->pieces[sq]);
        }
    }

    // Check the count
    for(int sq64 = 0; sq64 < 64; sq64++){
        int sq120 = SQ120(sq64);
        int piece = board->pieces[sq120];
        t_pceNum[piece]++;
        int color = PieceCol[piece];
        if(PieceBig[piece] == TRUE) t_bigPce[color]++;
        if(PieceMaj[piece] == TRUE) t_majorPce[color]++;
        if(PieceMin[piece] == TRUE) t_minorPce[color]++;

        t_material[color] += PieceVal[piece];

    }

    for(int p = wP; p <= bK; p++){
        ASSERT(board->pceNum[p] == t_pceNum[p]);
    }

    int pcount;
    pcount = CNT(t_pawns[WHITE]);
    ASSERT(pcount == board->pceNum[wP]);

    pcount = CNT(t_pawns[BLACK]);
    ASSERT(pcount == board->pceNum[bP]);

    pcount = CNT(t_pawns[BOTH]);
    ASSERT(pcount == (board->pceNum[wP] + board->pceNum[bP]));

    while(t_pawns[WHITE]){
        int sq64 = POP(&t_pawns[WHITE]);
        ASSERT(board->pieces[SQ120(sq64)] == wP);
    }


    while(t_pawns[BLACK]){
        int sq64 = POP(&t_pawns[BLACK]);
        ASSERT(board->pieces[SQ120(sq64)] == bP);
    }


    while(t_pawns[BOTH]){
        int sq64 = POP(&t_pawns[BOTH]);
        ASSERT(board->pieces[SQ120(sq64)] == wP || board->pieces[SQ120(sq64)] == bP);
    }


    ASSERT(t_material[WHITE] == board->material[WHITE] && t_material[BLACK] == board->material[BLACK]);
    ASSERT(t_bigPce[WHITE] == board->bigPce[WHITE] && t_bigPce[BLACK] == board->bigPce[BLACK]);
    ASSERT(t_majorPce[WHITE] == board->majorPce[WHITE] && t_majorPce[BLACK] == board->majorPce[BLACK]);
    ASSERT(t_minorPce[WHITE] == board->minorPce[WHITE] && t_minorPce[BLACK] == board->minorPce[BLACK]);

    ASSERT(board->side == WHITE || board->side == BLACK);
    ASSERT(generatePosKey(board) == board->posKey);

    ASSERT(board->enPass == NO_SQ ||(RanksBrd[board->enPass] == RANK_6 && board->side == WHITE) || (RanksBrd[board->enPass] == RANK_3 && board->side == BLACK));


    // Doubtish
    ASSERT(board->pieces[board->kingSq[WHITE]] == wK || board->pieces[board->kingSq[WHITE]] == OFF_BOARD);
    ASSERT(board->pieces[board->kingSq[BLACK]] == bK || board->pieces[board->kingSq[BLACK]] == OFF_BOARD);

    return TRUE;
}



void UpdateListsMaterial(S_BOARD *board){

    for(int index = 0; index < BRD_SQ_NUM; index++){
        int piece = board->pieces[index];
        if(piece != OFF_BOARD && piece != EMPTY){
            int colour =  PieceCol[piece];
            if(PieceBig[piece] == TRUE) board->bigPce[colour]++;
            if(PieceMin[piece] == TRUE) board->minorPce[colour]++;
            if(PieceMaj[piece] == TRUE) board->majorPce[colour]++;


            board->material[colour] += PieceVal[piece];

            board->pList[piece][board->pceNum[piece]] = index;
            board->pceNum[piece]++;

            if(piece == wK) board->kingSq[WHITE] = index;
            if(piece == bK) board->kingSq[BLACK] = index;


            if(piece == wP || piece == bP){
                SETBIT(board->pawns[colour], SQ64(index));
                SETBIT(board->pawns[BOTH], SQ64(index));
            }

        }
    }

}




int parseFen(const char *fen, S_BOARD *board){

    ASSERT(fen != NULL);
    ASSERT(board != NULL);

    int rank = RANK_8;
    int file = FILE_A;

    int frstSpace = FALSE;
    int index = 0;
    resetBoard(board);

    for(; index < strlen(fen); index++){       
        if(fen[index] == ' ') {
            break;
        }

        if(fen[index] != '/'){

            int fenPiece = getPiecefromChar(fen[index]);

            if(fenPiece == EMPTY){

                file += (fen[index] - '0') - 1;

            }else if(fenPiece != -1){
                
                board->pieces[FR2SQ(file, rank)] = fenPiece;

            }else{

                printf("FEN ERROR!");
                return 1;
            }

            file++;

        }else{
            if(file != FILE_NONE){
                printf("FEN ERROR!");
                return 1;
            }
            rank--;
            file = FILE_A;
        }
    }

    index++;

    ASSERT(fen[index] == 'w' || fen[index] == 'b');

    board->side = (fen[index] == 'w') ? WHITE : BLACK;
    index += 2;

    while(fen[index] != ' '){
        switch (fen[index])
        {
        case 'K':
            board->castlePerm |= WKCA;
            break;
        case 'Q':
            board->castlePerm |= WQCA;
            break;
        case 'k':
            board->castlePerm |= BKCA;
            break;
        case 'q':
            board->castlePerm |= BQCA;
            break;
        }
        index++;
    }

    index++;

    if(fen[index] != '-'){
        int enPassFile = fen[index] - 'a';
        int enPassRank = fen[index + 1] - '1';

        ASSERT(enPassFile >= FILE_A && enPassFile <= FILE_H);
        ASSERT(enPassFile >= RANK_1 && enPassFile <= RANK_8);

        board->enPass = FR2SQ(enPassFile, enPassRank);
        index+=2;
    }

    index++;

    board->posKey = generatePosKey(board);

    UpdateListsMaterial(board);
    ASSERT(CheckBoard(board));
    
    /*
    for(int i = 0;index < strlen(fen); index++){

        if(fen[index] == ' '){
            i++;
            continue;
        }

        switch (i)
        {
        case 0:
            if(fen[index] == 'w'){
                board->side = WHITE;
            }else{
                board->side = BLACK;
            }
            break;
        case 1:
            switch (fen[index])
            {
            case 'K':
                board->castlePerm |= WKCA;
                break;
            case 'Q':
                board->castlePerm |= WQCA;
                break;
            case 'k':
                board->castlePerm |= BKCA;
                break;
            case 'q':
                board->castlePerm |= BQCA;
                break;
            }
            break;
        case 2:
            if(fen[index] != '-'){
                int enPassFile = fen[index] - 'a';
                int enPassRank = fen[index + 1] - '1';
                board->enPass = FR2SQ(enPassFile, enPassRank);
                index++;
            }
            break;
        default:
            break;
        }

    }
    */




    return 0;
}



void resetBoard(S_BOARD *board){

    // Set all board squares to NO_SQ
    for(int i = 0; i < BRD_SQ_NUM; i++){
        board->pieces[i] = OFF_BOARD;
    }

    // Set all the 64 squares to EMPTY
    for(int i = 0; i < 64; i++){
        board->pieces[SQ120(i)] = EMPTY;
    }

    for(int i = WHITE; i <= BOTH; i++){
        board->pawns[i] = 0ULL;
    }

    for(int i = WHITE; i <= BLACK; i++){
        board->bigPce[i] = 0;
        board->majorPce[i] = 0;
        board->minorPce[i] = 0;
        board->material[i] = 0;
    }

    for(int i = wP; i <= bK; i++){
        board->pceNum[i] = 0;
    }

    board->kingSq[WHITE] = board->kingSq[BLACK] = NO_SQ;

    board->side = BOTH;
    board->enPass = NO_SQ;
    board->fiftyMoves = 0;
    board->ply = 0;
    board->hisply = 0;
    board->castlePerm = 0;
    board->posKey = 0ULL;

    InitPvTable(board->pvTable);

}

void printBoard(const S_BOARD *board){

    printf("GAME BOARD \n\n");
    
    for(int rank = RANK_8; rank >= RANK_1; rank--){
        printf("%8c", RankChars[rank]);
        for(int file = FILE_A; file <= FILE_H; file++){
            int piece = board->pieces[FR2SQ(file, rank)];
            printf("%5c", PieceChars[piece]);
        }
        printf("\n\n");
    }
    

    printf("\n%8c", ' ');
    for(int file = FILE_A; file <= FILE_H; file++) printf("%5c", FileChars[file]);
    printf("\n\n");

    printf("Side to Play : %c\n", SideChars[board->side]);
    printf("EnPass : %d\n", board->enPass);
    printf("Castle Perm : %c%c%c%c\n",
        board->castlePerm & WKCA ? 'K' : '-',
        board->castlePerm & WQCA ? 'Q' : '-',
        board->castlePerm & BKCA ? 'k' : '-',
        board->castlePerm & BQCA ? 'q' : '-'
    );

    printf("PosKey : %llX\n", board->posKey);
    printf("\n\n");
}
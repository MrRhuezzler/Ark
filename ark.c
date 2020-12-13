#include "ark.h"

#define FEN "k2qr3/5PPP/8/8/8/8/8/KR3Q2 w - - 0 1"

int main(){

    Init();

    S_BOARD board[1];

    parseFen(FEN, board);

    char input[6];
    int move;

    while (TRUE)
    {
        printBoard(board);
        printf("Enter a move > ");
        gets(input);

        if(input[0] == 'q'){
            break;
        }else if(input[0] == 't'){
            TakeMove(board);
        }else if(input[0] == 'p'){
            // perftTest(4, board);
            int count = GetPvLine(board, 4);
            for(int i = 0; i < count; i++){
                printf("%s, ", PrMove(board->pvArray[i]));
            }
            printf("\n");
        }else{
            move = ParseInputMove(input, board);
            if(move != NOMOVE){
                StorePvTable(board, move);
                MakeMove(board, move);
                if(IsRepetition(board)){
                    printf("\n\nREPETITION !\n\n");
                }
            }else{
                printf("Move not parsed correctly!\n");
            }
        }

        fflush(stdin);
    }



    return 0;
}
#include "ark.h"

#define INPUTBUFFER 400 * 6

#define NAME "Ark"
#define AUTHOR "MrRhuezzler"

#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

// go depth 6 wtime 180000 btime 100000 binc 1000 winc 1000 movetime 1000 movestogo 40
void ParseGo(char* line, S_SEARCHINFO* info, S_BOARD* board) {

	int depth = -1, movestogo = 30, movetime = -1;
	int time = -1, inc = 0;
	char* ptr = NULL;
	info->timeset = FALSE;

	if ((ptr = strstr(line, "infinite"))) {
		;
	}

	if ((ptr = strstr(line, "binc")) && board->side == BLACK) {
		inc = atoi(ptr + 5);
	}

	if ((ptr = strstr(line, "winc")) && board->side == WHITE) {
		inc = atoi(ptr + 5);
	}

	if ((ptr = strstr(line, "wtime")) && board->side == WHITE) {
		time = atoi(ptr + 6);
	}

	if ((ptr = strstr(line, "btime")) && board->side == BLACK) {
		time = atoi(ptr + 6);
	}

	if ((ptr = strstr(line, "movestogo"))) {
		movestogo = atoi(ptr + 10);
	}

	if ((ptr = strstr(line, "movetime"))) {
		movetime = atoi(ptr + 9);
	}

	if ((ptr = strstr(line, "depth"))) {
		depth = atoi(ptr + 6);
	}

	if (movetime != -1) {
		time = movetime;
		movestogo = 1;
	}

	info->starttime = getMillis();
	info->depth = depth;

	if (time != -1) {
		info->timeset = TRUE;
		time /= movestogo;
		time -= 50;
		info->stoptime = info->starttime + (U64)time + (U64)inc;
	}

	if (depth == -1) {
		info->depth = MAX_DEPTH;
	}
	printf("time:%d start:%lld stop:%lld depth:%d timeset:%d\n",
		time, info->starttime, info->stoptime, info->depth, info->timeset);
	SearchPosition(board, info);
}


void ParsePos(char* lineIn, S_BOARD* board) {

	lineIn += 9;
	char* ptrChar = lineIn;

	if (strncmp(lineIn, "startpos", 8) == 0) {
		parseFen(START_FEN, board);
	}
	else {
		ptrChar = strstr(lineIn, "fen");
		if (ptrChar == NULL) {
			parseFen(START_FEN, board);
		}
		else {
			ptrChar += 4;
			parseFen(ptrChar, board);
		}
	}

	ptrChar = strstr(lineIn, "moves");
	int move;

	if (ptrChar != NULL) {
		ptrChar += 6;
		while (*ptrChar) {
			move = ParseInputMove(ptrChar, board);
			if (move == NOMOVE) break;
			MakeMove(board, move);
			board->ply = 0;
			while (*ptrChar && *ptrChar != ' ') ptrChar++;
			ptrChar++;
		}
	}
	printBoard(board);
}

void matein_(S_BOARD *board, S_SEARCHINFO *info, int mate_moves){

	FILE *fp;

	char filename[100];
	sprintf(filename, "MateIn%d.txt", mate_moves);

	fp = fopen(filename, "r");

	if(fp == NULL){
		printf("Couldn't find the problem sheet\n");
		return;
	}

	char buffer[250];
	char fenstr[250];
	int choice;

	do{
		for(int i = 0; i < 3; i++){
			switch (i)
			{
			case 0:
				fgets(buffer, 250, fp);
				printf("\nGame : %s", buffer);
				break;
			case 1:
				fgets(fenstr, 250, fp);
				printf("FEN : %s", fenstr);
				break;
			case 2:
				fgets(buffer, 250, fp);
				printf("BestMoves : %s\n", buffer);
				break;
			}
		}

		for(int i = 0; i < 2; i++){
			fgets(buffer, 250, fp);
		}

		printf("Enter 1 to Skip : ");
		scanf("%d", &choice);
		if(choice == 1) continue;

		parseFen(fenstr, board);
		printBoardOnly(board);
		
		if (mate_moves >= 4) {
			info->depth = 8;
		}
		else {
			info->depth = mate_moves * 2;
		}

		SearchPosition(board, info);

		int total = GetPvLine(board, info->depth);
        printf("Moves : ");
		char * moveNotation;
		int moveNumber = 1;
		
		if(board->side == BLACK){
			printf("%d... ", moveNumber++);
		}

        for(int i = 0; i < total; ++i){
			if(board->side == WHITE){
				printf("%d. ", moveNumber++);
			}
			moveNotation = HNPrMove(board->pvArray[i], board);
			for(int i = 0; i < 8; i++){
				if(moveNotation[i] != '\0'){
					printf("%c", moveNotation[i]);
				}
			}
			printf(" ");
        }

		while(total--){
			TakeMove(board);
		}

        printf("\n");

		printf("Enter 1 to Continue : ");
		scanf("%d", &choice);

	}while((choice == 1) && (!feof(fp)));

}

void UCILoop() {
	
	setbuf(stdin, NULL);
	setbuf(stdout, NULL);


	char line[INPUTBUFFER];
	printf("id name %s\n", NAME);
	printf("id author %s\n", AUTHOR);
	printf("uciok\n");

	S_BOARD board[1];
	S_SEARCHINFO info[1];
	info->quit = FALSE;
	info->stopped = FALSE;
	InitPvTable(board->pvTable);


	while (TRUE) {
		
		memset(&line[0], 0, sizeof(line));
		fflush(stdout);
		if (!fgets(line, INPUTBUFFER, stdin)) continue;

		if (line[0] == '\n') continue;


		if (!strncmp(line, "isready", 7)) {
			printf("isreadyok\n");
		}
		else if (!strncmp(line, "position", 8)) {
			ParsePos(line, board);
		}
		else if (!strncmp(line, "go", 2)) {
			ParseGo(line, info, board);
		}
		else if (!strncmp(line, "ucinewgame", 10)) {
			ParsePos("position startpos\n", board);
		}
		else if (!strncmp(line, "uci", 3)) {
			printf("id name %s\n", NAME);
			printf("id author %s\n", AUTHOR);
			printf("uciok\n");
		}
		else if (!strncmp(line, "quit", 4)) {
			info->quit = TRUE;
			break;
		}
		#ifdef BETA
		else if(!strncmp(line, "analysis", 7)){
			int mate_moves;
			printf("Mate in : ");
			scanf("%d", &mate_moves);

			matein_(board, info, mate_moves);
		}
		#endif
		if (info->quit) {
			break;
		}


	}

	free(board->pvTable->pTable);

}
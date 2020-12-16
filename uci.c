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
// position fen FENSTRING
// position startpos
// .... moves e2e4 ...

/*
void ParsePos(char* line, S_BOARD* board) {
	
	int index = 9;

	if (!strncmp(line + index, "fen", 3)) {
		
		// position fen FENSTR moves
		// position fen FENSTR 00000
		char FENSTR[150];
		index = 13;
		int i = 0;
		
		while (line[index] != '\0' && line[index] != 'm') {
			FENSTR[i++] = line[index++];
		}

		parseFen(FENSTR, board);
	}
	else {

		// position startpos

		parseFen(START_FEN, board);
		index = 18;
	}

	if (!strncmp(line + index, "moves", 5)) {
		index += 6;
		int move = NOMOVE;
		char moves[6];
		int i = 0;

		while (line[index] != '\0') {
			if (line[index] == ' ') {
				i = 0;
				move = ParseInputMove(moves, board);
				if (move != NOMOVE) {
					MakeMove(board, move);
					board->ply = 0;
				}
			}
			else {
				moves[i++] = line[index];
			}
			index++;
		}

		move = ParseInputMove(moves, board);
		if (move != NOMOVE) {
			MakeMove(board, move);
			board->ply = 0;
		}
	}

	printBoard(board);

}
*/
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

		if (info->quit) {
			break;
		}


	}

	free(board->pvTable->pTable);

}
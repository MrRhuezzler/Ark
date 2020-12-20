#include "ark.h"

// FEN String Representing Chess Positions
#define MATEIN3 "2rr3k/pp3pp1/1nnqbN1p/3pN3/2pP4/2P3Q1/PPB4P/R4RK1 w - -"
#define MATEIN5 "2q1nk1r/4Rp2/1ppp1P2/6Pp/3p1B2/3P3P/PPP1Q3/6K1 w - - 0 1"

int main(){

    // Initializes the Engine
    Init();

    // Uci Protocol Loop
    UCILoop();

    return 0;
}
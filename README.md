![ARK](/images/header.png)
---
Author           : [MrRhuezzler](https://github.com/MrRhuezzler)  
Project Language : C  
Project Year     : 2020  
Status           : :white_check_mark: (Working) 

Inspired by Bluefever !

## Ark Mating me in 4 moves as Black
![CHESS_GIF](/images/Chess.gif)

- ## Progress :
    - [X] Board Representation
    - [X] FEN Parser
    - [X] Postion Setup
    - [X] Attacked Square
    - [X] Move Structure
    - [X] Move Generator
    - [X] Make Move
    - [X] Perf Testing
    - [X] Do / Undo Moves
    - [X] Skeleton Evaluation ( Milestone )
    - [X] Search Algorithm
    - [X] GUI Connection
        - [X] [UCI Protocol](http://wbec-ridderkerk.nl/html/UCIProtocol.html)

## Details

### Board Representation
![ChessBoardRepresentation](/images/ChessBoardRep.jpg)

### Forsyth–Edwards Notation (FEN) String 
A FEN String is a way to encode a chess position in a single string.  
- Rules
    - Example, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
    - Each Rank is divided by a "/", starting from the 8th rank decreasing to the 1st Rank.
    - Within each rank, each square is described from file 'a' to 'h'.
    - Following the SAN, each piece is denoted by a single character.
        - Small Letters denote Black Pieces
        - Captials denote White Pieces
        - r - Rook, b - bishop, k - king, n - knight, q - queen, p - pawn
    - A group of empty squares are denoted by the decimal number, which denotes the total number of consecutive empty square.
    - Following this side to play comes
    - After that we have the castle permission
        - K - White Side King Castling
        - Q - White Side Queen Castling
        - k - Black Side King Castling
        - q - Black Side Queen Castling
        - If not present "-" is given
    
    - Then the enpassment square, if not available "-" is passed
    - Then two numbers first one denoted the total number of half moves played,
    - The Second number denoted the number of moves played.

Starting Position in FEN, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"  

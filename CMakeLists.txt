cmake_minimum_required(VERSION 3.17)
project(Ark)
add_executable("Ark" "init.c" "bitboard.c" "board.c" "hashkey.c" "attack.c" "io.c" "movegen.c" "makemove.c" "validate.c" "ark.c" "ark.h")

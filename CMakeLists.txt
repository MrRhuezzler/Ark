cmake_minimum_required(VERSION 3.17)
project(Ark)
include_directories("./include")
# add_compile_definitions(BETA)

add_executable("Ark" "init.c" "bitboard.c" "board.c" "hashkey.c" "attack.c" "io.c" "movegen.c" "makemove.c" "validate.c" "perft.c" "search.c" "misc.c" "pvtable.c" "evaluate.c" "uci.c" "ark.c")

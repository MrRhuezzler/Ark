cmake_minimum_required(VERSION 3.17)
project(Ark)

# if(CMAKE_BUILD_TYPE STREQUAL "Debug")
add_compile_definitions(WIN32)
# endif()

add_executable("Ark" "init.c" "bitboard.c" "board.c" "hashkey.c" "attack.c" "io.c" "movegen.c" "makemove.c" "validate.c" "perft.c" "search.c" "misc.c" "pvtable.c" "ark.c" "ark.h")

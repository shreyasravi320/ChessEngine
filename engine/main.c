#include "include/board.h"

int main(int argc, char *argv[])
{
    Board board = 0ULL;

    set(board, e4);
    set(board, h1);
    set(board, g3);
    set(board, a2);

    printBoard(board);
    return 0;
}
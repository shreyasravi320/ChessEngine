#include "include/logic.h"
#include <iostream>
using std::cout;

int main()
{
    init();
    parseFen(promPos);
    printBoard();
    generateMoves();
    return 0;
}
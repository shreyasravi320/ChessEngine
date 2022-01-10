#include "include/logic.h"
#include <iostream>
using std::cout;

int main()
{
    init();
    parseFen(trickyPos);
    printBoard();
    printAttackedSquares(WHITE);

    return 0;
}
#include "include/search.h"

int main()
{
    initAttacks();

    parseFen(POS_1);
    printBoard();
    engine(5);
    
    return 0;
}
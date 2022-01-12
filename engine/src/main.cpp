#include "include/perft.h"

int main()
{
    initAttacks();

    parseFen(POS_1);
    perftTest(5);

    return 0;
}